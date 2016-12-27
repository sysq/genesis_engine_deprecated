/** \file simplegamestatehandler.h
* \brief 文件内容说明
* Copyright (c) 2011,畅游天下WebJet事业部
* All rights reserved.
* Date        Ver    Author          Comment
* 2011:11:16   1.0    libin   
*/
#ifndef __simplegamestatehandler_H__
#define __simplegamestatehandler_H__

#include "appframework/statehandler.h"
#include "appframework/actor.h"
#include "math/ray.h"
#include "math/color.h"
#include "gyrostat.h"
#include "particles/particlesystem.h"

#include "guifeature/gui.h"

namespace Sample
{
	using namespace Particles;

	class SimpleStateHandler : public App::StateHandler
	{
		__DeclareClass(SimpleStateHandler);
	public:    

		/// constructor
		SimpleStateHandler();
		/// destructor
		virtual ~SimpleStateHandler();

		//static Func for Web 
		static void ChangeParticleUp();
		static void ChangeParticleDown();

		/// called when the state represented by this state handler is entered
		virtual void OnStateEnter(const Util::String& prevState);
		/// called when the state represented by this state handler is left
		virtual void OnStateLeave(const Util::String& nextState);
		/// called each frame as long as state is current, return new state
		virtual Util::String OnFrame();

		/// set level filename, required by setup mode LoadLevel
		void SetSceneName(const Util::String& n);
		/// get level name
		const Util::String& GetSceneName() const;

		//示例坐标系
		void UpdateWorldCoord(void);

		// 示例：如何进行拾取操作
		void IntersectActor();

		// 示例：更新绘制线的数据
		void UpdateLine(void);

		//创建更新陀螺
		void UpdateGyrostat();

		//check circle actor
		bool IsCircleSelected(const Ptr<App::Actor>& actor );


		// 示例：缩放士兵
		void UpdateSoldier(void);

		// 示例，挂接操作
		void ProcessAttach();

		// 绘制出拾取到的actor的包围盒
		void CreateOrUpdateBoundActor(const Ptr<App::Actor>& actor );

		void CreateParticleFromTemplate();

		void CreateScene();

		//
		void SetKeyDown(int key);

		void ChangeEmitter();

		//
		void createParSys(Util::Array<Util::StringAtom>& outTexture);

		void LoadTemplate();

		void UpdateParticle();

		void InitParticleTexutre(Util::Array<Util::StringAtom>& outTexture);


	private:    
		Util::String mSceneName;

		App::Actor::FastId mMeshActorID;
		Util::String mCurMeshName;
		Util::String mCurShaderName;
		Util::String mCurTexName;

		bool mShowAll;	//	显示本身的包围盒，还是包含孩子节点的包围盒
		bool mShowParticle;

		Ptr<App::Scene> mScene;

		Ptr<App::Actor> mCamera;
		Ptr<App::Actor> mLight;
		Ptr<App::Actor> mLineActor;		//	绘制线示例
		Ptr<App::Actor> mSoldierActor;	//	绘制大兵
		Ptr<App::Actor> mSoldierActorChild;

		Ptr<App::Actor> mSelectActor;	//	当前选中的actor
		Ptr<App::Actor> mIntersectBoundActor;	//	用于显示包围盒的actor

		Ptr<App::Actor> mWorldCoordsActor;	//	参考坐标
		//mGyrostatActor //need attach a circleActors , acoordsActor, three taper,


		Ptr<App::Actor> mSelectOpActor;	//	当前选中的陀螺仪actor
		Gyrostat* mGyrostat;

		float2	mMouseScreenPos;//上一次鼠标位置

		Ptr<App::Actor> ParticleAct;
		Util::Array<Util::StringAtom> outlist;
		Ptr<ParticleSystem> ParSystem;


		// ===================== UI test ======================
		void _initUI();
		bool _frameUI();
		MyGUI::VectorWidgetPtr _addLayout(char* name);
		void _emptyWindowClick(MyGUI::Widget* _sender);
		void _radioClick(MyGUI::Widget* _sender);
		void _checkClick(MyGUI::Widget* _sender);
		void _nextPtc(MyGUI::Widget* _sender);
		void _prevPtc(MyGUI::Widget* _sender);
		void _clearPtc(MyGUI::Widget* _sender);

		void _requestCreateWidgetItem(MyGUI::ItemBox* _sender, MyGUI::Widget* _item);
		void _notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position);

		MyGUI::VectorWidgetPtr mEmptyWin;
		MyGUI::Button* mEmtyWinBtn;
		MyGUI::EditBox* mPressTxt;
		MyGUI::ScrollBar* mRed;
		MyGUI::ScrollBar* mGreen;
		MyGUI::ScrollBar* mBlue;
		MyGUI::Widget* mColor;
		MyGUI::EditBox* mColorText;

		Util::Array<MyGUI::Button*> mRadios;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
	SimpleStateHandler::SetSceneName(const Util::String& n)
	{
		this->mSceneName = n;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline const Util::String&
	SimpleStateHandler::GetSceneName() const
	{

		return this->mSceneName;
	}
}



#endif // __simplegamestatehandler_H__



