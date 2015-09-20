#ifndef __FSM_H_
#define __FSM_H_
#include "util.h"

namespace olib {
	template <typename T, typename E>
	class State {
	public:
		virtual ~State() {}

		virtual void OnEnter(T* entity) = 0;
		virtual void OnLeave(T* entity) = 0;
		virtual void OnEvent(T* entity, const E& event) = 0;
	};

	template <class T, class E>
	class StateMachine {
	public:
		StateMachine(T* owner) : _owner(owner), _currentState(nullptr) {}
		virtual ~StateMachine() {}

		inline void Initialize(State<T, E>* state) {
			_currentState = state;
		}

		inline State<T, E>* GetCurrentState() const {
			return _currentState;
		}

		inline const void OnEvent(const E& event) {
			OASSERT(_currentState, "where is current state");
			_currentState->OnEvent(_owner, event);
		}

		inline const void ChangeState(State<T, E>* state) {
			OASSERT(_currentState && state, "where is current or new");
			_currentState->OnLeave(_owner);
			_currentState = state;
			_currentState->OnEnter(_owner);
		}

	private:
		T * _owner;
		State<T, E>* _currentState;
	};
}

#endif //__FSM_H_
