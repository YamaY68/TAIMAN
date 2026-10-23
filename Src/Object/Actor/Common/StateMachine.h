#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "IState.h"

template <typename T>
class StateMachine
{
public:
    explicit StateMachine(T* owner) : owner_(owner), currentState_(nullptr) {}

    // ステートの事前登録
    template <typename S, typename... Args>
    void RegisterState(Args&&... args)
    {
        std::type_index type = std::type_index(typeid(S));
        if (states_.find(type) == states_.end())
        {
            states_[type] = std::make_unique<S>(std::forward<Args>(args)...);
        }
    }

    // 型指定によるステート遷移
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

    void HandleInput()
    {
        if (currentState_) currentState_->HandleInput(owner_);
    }

    void Update()
    {
        if (currentState_) currentState_->Update(owner_);
    }

    IState<T>* GetCurrentState() const { return currentState_; }
    const char* GetCurrentStateName() const { return currentState_ ? currentState_->GetName() : "None"; }

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