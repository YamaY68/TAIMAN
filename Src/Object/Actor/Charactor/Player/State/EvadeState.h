#pragma once
#include "../../../Common/IState.h"
#include <DxLib.h>

class ActorBase;

class EvadeState : public IState<ActorBase>
{
public:
    //進入
    void Enter(ActorBase* owner) override;
    //入力受付
    void HandleInput(ActorBase* owner) override;
    //更新
    void Update(ActorBase* owner) override;
    //離脱
    void Exit(ActorBase* owner) override;
    /// <summary>
    /// 回避方向の設定
    /// </summary>
    /// <param name="dir">回避方向</param>
    void SetEvadeDirection(const VECTOR& dir) { evadeDirection_ = dir; }

private:
    VECTOR evadeDirection_ = { 0.0f, 0.0f, 0.0f };
    bool isDive_ = false;
};