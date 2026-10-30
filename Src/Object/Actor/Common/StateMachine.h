#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "IState.h"

template <typename T>
class StateMachine
{
public:
    //コンストラクタ
    explicit StateMachine(T* owner) : owner_(owner), currentState_(nullptr) {}

    /// <summary>
    /// ステートの登録
    /// </summary>
    /// <typeparam name="S">型名</typeparam>
    /// <typeparam name="...Args">引数型</typeparam>
    /// <param name="...args">引数</param>
    template <typename S, typename... Args>
    void RegisterState(Args&&... args)
    {
        std::type_index type = std::type_index(typeid(S));
        if (states_.find(type) == states_.end())
        {
            states_[type] = std::make_unique<S>(std::forward<Args>(args)...);
        }
    }

    /// <summary>
    /// 型指定によるステート遷移
    /// </summary>
    /// <typeparam name="S">遷移するステート型名</typeparam>
    template <typename S>
    void ChangeState()
    {
        std::type_index type = std::type_index(typeid(S));
        auto it = states_.find(type);
        if (it != states_.end())
        {
            ChangeStateInternal(it->second.get());
        }
    }
    //入力処理
    void HandleInput()
    {
        if (currentState_) currentState_->HandleInput(owner_);
    }
    //更新
    void Update()
    {
        if (currentState_) currentState_->Update(owner_);
    }
    ///現在ステートの取得
    IState<T>* GetCurrentState() const { return currentState_; }
    //現在ステート名の取得
    const char* GetCurrentStateName() const { return currentState_ ? currentState_->GetName() : "None"; }
    /// <summary>
    /// 登録済みステートの取得
    /// </summary>
    /// <typeparam name="S">型名</typeparam>
    /// <param name=""></param>
    /// <returns></returns>
    template <typename S>
    S* GetState(void)const
    {
        auto it = states_.find(typeid(S));
        if (it != states_.end())
        {
            return dynamic_cast<S*>(it->second.get());
        }
        return nullptr;
    }
private:
    void ChangeStateInternal(IState<T>* nextState)
    {
        if (currentState_)
        {
            currentState_->Exit(owner_);
        }
        currentState_ = nextState;
        if (currentState_)
        {
            currentState_->Enter(owner_);
        }
    }

private:
    T* owner_;
    IState<T>* currentState_;
    std::unordered_map<std::type_index, std::unique_ptr<IState<T>>> states_;
};