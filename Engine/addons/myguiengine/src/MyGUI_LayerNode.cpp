/*!
	@file
	@author		Albert Semenov
	@date		02/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_ILayerItem.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_ISubWidgetText.h"

namespace MyGUI
{

	LayerNode::LayerNode(ILayer* _layer, ILayerNode* _parent) :
		mParent(_parent),
		mLayer(_layer),
		mOutOfDate(false)
	{
	}

	LayerNode::~LayerNode()
	{
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
			delete (*iter);
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
			delete (*iter);
		mSecondRenderItems.clear();

		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
			delete (*iter);
		mChildItems.clear();
	}

	ILayerNode* LayerNode::createChildItemNode()
	{
		LayerNode* layer = new LayerNode(mLayer, this);
		mChildItems.push_back(layer);

		mOutOfDate = true;

		return layer;
	}

	void LayerNode::destroyChildItemNode(ILayerNode* _node)
	{
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			if ((*iter) == _node)
			{
				delete _node;
				mChildItems.erase(iter);

				mOutOfDate = true;

				return;
			}
		}
		MYGUI_EXCEPT("item node not found");
	}

	void LayerNode::upChildItemNode(ILayerNode* _item)
	{
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				mChildItems.erase(iter);
				mChildItems.push_back(_item);

				mOutOfDate = true;

				return;
			}
		}
		MYGUI_EXCEPT("item node not found");
	}

	void LayerNode::renderToTarget(IRenderTarget* _target, bool _update)
	{
		// проверяем на сжатие пустот
		bool need_compression = false;
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
		{
			if ((*iter)->getCompression())
			{
				need_compression = true;
				break;
			}
		}

		if (need_compression)
			updateCompression();

		// сначала отрисовываем свое
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
			(*iter)->renderToTarget(_target, _update);

		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
			(*iter)->renderToTarget(_target, _update);

		// теперь отрисовываем дочерние узлы
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
			(*iter)->renderToTarget(_target, _update);

		mOutOfDate = false;
	}

	void LayerNode::resizeView(const IntSize& _viewSize)
	{
		IntSize oldSize = mLayer->getSize();

		for (VectorLayerItem::const_iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
			(*iter)->resizeLayerItemView(oldSize, _viewSize);
	}

	ILayerItem* LayerNode::getLayerItemByPoint(int _left, int _top) const
	{
		// сначала пикаем детей
		for (VectorILayerNode::const_iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item)
				return item;
		}

		for (VectorLayerItem::const_iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item)
				return item;
		}

		return nullptr;
	}

	RenderItem* LayerNode::addToRenderItem(ISubWidget* _subWidget, ITexture* _texture, bool _firstQueue, bool _separate)
	{
		RenderItem* item = getRenderItem(_texture, _firstQueue, _separate);
		item->addDrawItem(_subWidget, _subWidget->getVertexCount());
		return item;
	}

	RenderItem* LayerNode::updateRenderItem(ISubWidget* _subWidget, RenderItem* _itemOld, ITexture* _texNew, bool _firstQueue, bool _separate)
	{
		if (_itemOld == nullptr)
		{
			return addToRenderItem(_subWidget, _texNew, _firstQueue, _separate);
		}
		bool separateOld = _itemOld->getManualRender();

		// check in first array.
		VectorRenderItem::iterator itOld = std::find(mFirstRenderItems.begin(), mFirstRenderItems.end(), _itemOld);
		if (itOld != mFirstRenderItems.end())
		{	
			ITexture* _texOld = _itemOld->getTexture();
			if (_texOld == _texNew && separateOld == _separate)
			{
				return _itemOld;
			}
			else
			{
				// break render item.
				VectorDrawItem frontDrawItems;
				VectorDrawItem tailDrawItems;

				(*itOld)->breakDrawItems(_subWidget, frontDrawItems, tailDrawItems);

				// front
				if (frontDrawItems.size())
				{
					itOld = insertNewRenderItem(_texOld, separateOld, frontDrawItems, mFirstRenderItems, itOld);
					RenderItem* itemFront = *itOld;
					++itOld; //roll back to insert node,
					notifyRenderItemChanged(frontDrawItems, _itemOld, itemFront);
				}

				//tail
				if (tailDrawItems.size())
				{
					itOld = insertNewRenderItem(_texOld, separateOld, tailDrawItems, mFirstRenderItems, itOld + 1);
					RenderItem* itemTail = *itOld;
					--itOld;//roll back to insert node,
					notifyRenderItemChanged(tailDrawItems, _itemOld, itemTail);
				}

				// middle
				mOutOfDate = false;

				if (0 == frontDrawItems.size())
				{
					if (*itOld != mFirstRenderItems.front())
					{
						// try combine vertices.
						VectorRenderItem::iterator preItem = itOld - 1;
						if ((*preItem)->getTexture() == _texNew && (((*preItem)->getManualRender() || _separate) == false) ) // manual render cannot combine vertices
						{
							(*preItem)->addDrawItem(_subWidget, _subWidget->getVertexCount());
							_subWidget->onRenderItemChanged(this, _itemOld, *preItem);

							(*itOld)->clearDrawItems();
							return *preItem;
						}
					}
				}

				if (0 == tailDrawItems.size())
				{
					if (*itOld != mFirstRenderItems.back())
					{
						// try combine vertices.
						VectorRenderItem::iterator nextItem = itOld + 1;
						if ((*nextItem)->getTexture() == _texNew && ((*nextItem)->getManualRender() || _separate == false) ) // manual render cannot combine vertices.
						{
							(*nextItem)->insertDrawItem(0, _subWidget, _subWidget->getVertexCount());
							_subWidget->onRenderItemChanged(this, _itemOld, *nextItem);

							(*itOld)->clearDrawItems();
							return *nextItem;
						}
					}
				}
		
				(*itOld)->clearDrawItems();
				(*itOld)->setManualRender(_separate);
				(*itOld)->setTexture(_texNew);
				(*itOld)->addDrawItem(_subWidget, _subWidget->getVertexCount());
				return *itOld;
			}
		}

		// check in second array.
		_itemOld->removeDrawItem(_subWidget);
		return addToRenderItem(_subWidget, _texNew, _firstQueue, _separate);
	}

	RenderItem* LayerNode::getRenderItem(ITexture* _texture, bool _firstQueue, bool _manualRender)
	{
		// для первичной очереди нужен порядок
		if (_firstQueue)
		{
			if (mFirstRenderItems.empty() || _manualRender)
			{
				return pushNewRenderItem(mFirstRenderItems, _texture, _manualRender);
			}

			// find first empty element.
			// если в конце пустой буфер, то нуна найти последний пустой с краю
			// либо с нужной текстурой за пустым
			VectorRenderItem::reverse_iterator iter = mFirstRenderItems.rbegin();
			if ((*iter)->getDrawItemCount() == 0)
			{
				while (true)
				{
					VectorRenderItem::reverse_iterator next = iter + 1;
					if (next != mFirstRenderItems.rend())
					{
						if ((*next)->getDrawItemCount() == 0)
						{
							iter = next;
							continue;
						}
						else if ((*next)->getTexture() == _texture && (!(*iter)->getManualRender()) )
						{
							iter = next;
						}
					}

					break;
				}

				(*iter)->setTexture(_texture);
				(*iter)->setManualRender(_manualRender);
				mOutOfDate = false;

				return (*iter);
			}
			// последний буфер с нужной текстурой
			else if ( (*iter)->getTexture() == _texture && (!(*iter)->getManualRender()) )
			{
				mOutOfDate = false;

				return *iter;
			}
			return pushNewRenderItem(mFirstRenderItems, _texture, _manualRender);
		}

		// для второй очереди порядок неважен
		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
		{
			// либо такая же текстура, либо пустой буфер
			if ( (*iter)->getTexture() == _texture && (!(*iter)->getManualRender()) )
			{
				mOutOfDate = false;

				return (*iter);
			}
			else if ((*iter)->getDrawItemCount() == 0)
			{
				(*iter)->setTexture(_texture);
				(*iter)->setManualRender(_manualRender);
				mOutOfDate = false;

				return (*iter);
			}
		}

		return pushNewRenderItem(mSecondRenderItems, _texture, _manualRender);
	}

	RenderItem* LayerNode::newRenderItem(ITexture* _texture, bool _manualRender)
	{
		RenderItem* item = new RenderItem(this);
		item->setTexture(_texture);
		item->setManualRender(_manualRender);
		return item;
	}

	VectorRenderItem::iterator LayerNode::insertNewRenderItem(ITexture* _texture, bool _manualRender, const VectorDrawItem& _nodes,
		VectorRenderItem& _array, const VectorRenderItem::iterator& _where)
	{
		RenderItem* _item = nullptr;

		if (_array.size())
		{
			if (_array.back()->getDrawItemCount() == 0)
			{
				_item = _array.back();
				_array.pop_back();
				_item->setManualRender(_manualRender);
				_item->setTexture(_texture);
			}
		}

		if (nullptr == _item)
		{
			_item = newRenderItem(_texture, _manualRender);
		}

		VectorDrawItem::const_iterator itf = _nodes.begin();
		VectorDrawItem::const_iterator endf = _nodes.end();
		while(itf != endf)
		{
			_item->addDrawItem(itf->first, itf->second);
			++itf;
		}
		return _array.insert(_where, _item);
	}

	RenderItem* LayerNode::pushNewRenderItem(VectorRenderItem& _array, ITexture* _texture, bool _manualRender)
	{
		RenderItem* item = newRenderItem(_texture, _manualRender);
		mOutOfDate = false;
		_array.push_back(item);
		return _array.back();
	}

	void LayerNode::notifyRenderItemChanged(const VectorDrawItem& _nodes, RenderItem* _old, RenderItem* _new)
	{
		VectorDrawItem::const_iterator itt = _nodes.begin();
		VectorDrawItem::const_iterator endt = _nodes.end();
		while(itt != endt)
		{
			itt->first->onRenderItemChanged(this, _old, _new);
			++itt;
		}
	}

	void LayerNode::attachLayerItem(ILayerItem* _item)
	{
		mLayerItems.push_back(_item);
		_item->attachItemToNode(mLayer, this);

		mOutOfDate = true;
	}

	void LayerNode::detachLayerItem(ILayerItem* _item)
	{
		for (VectorLayerItem::iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				mLayerItems.erase(iter);

				mOutOfDate = true;

				return;
			}
		}
		MYGUI_EXCEPT("layer item not found");
	}

	void LayerNode::outOfDate(RenderItem* _item)
	{
		mOutOfDate = true;
		if (_item)
			_item->outOfDate();
	}

	EnumeratorILayerNode LayerNode::getEnumerator() const
	{
		return EnumeratorILayerNode(mChildItems);
	}

	void LayerNode::updateCompression()
	{
		// буферы освобождаются по одному всегда
		if (mFirstRenderItems.size() > 1)
		{
			// пытаемся поднять пустой буфер выше полных
			VectorRenderItem::iterator iter1 = mFirstRenderItems.begin();
			VectorRenderItem::iterator iter2 = iter1 + 1;
			while (iter2 != mFirstRenderItems.end())
			{
				if ((*iter1)->getDrawItemCount() == 0)//(*iter1)->getNeedVertexCount() == 0 && !(*iter1)->getManualRender())
				{
					RenderItem* tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
		}

		mOutOfDate = true;
	}

	ILayer* LayerNode::getLayer() const
	{
		return mLayer;
	}

	ILayerNode* LayerNode::getParent() const
	{
		return mParent;
	}

	bool LayerNode::isOutOfDate() const
	{
		for (VectorRenderItem::const_iterator item = mFirstRenderItems.begin(); item != mFirstRenderItems.end(); ++item)
		{
			if ((*item)->isOutOfDate())
				return true;
		}

		for (VectorRenderItem::const_iterator item = mSecondRenderItems.begin(); item != mSecondRenderItems.end(); ++item)
		{
			if ((*item)->isOutOfDate())
				return true;
		}

		for (VectorILayerNode::const_iterator item = mChildItems.begin(); item != mChildItems.end(); ++item)
		{
			if (static_cast<const LayerNode*>(*item)->isOutOfDate())
				return true;
		}

		return mOutOfDate;
	}

} // namespace MyGUI
