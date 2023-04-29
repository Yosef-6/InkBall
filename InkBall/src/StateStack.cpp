#include "StateStack.h"
#include <cassert>

StateStack::StateStack(sf::RenderWindow& window):m_Stack(),m_PendingList(),m_window(&window),m_Factories()
{
}

void StateStack::update(sf::Time dt)
{
    
    for (auto itr = m_Stack.rbegin(); itr != m_Stack.rend(); ++itr)
    {
        if (!(*itr)->update(dt))
            break;
    }

    applyPendingChanges();
}

void StateStack::draw()
{
    for (const auto& state : m_Stack)
        state->draw();

}

void StateStack::handleEvent(const sf::Event& event)
{
    for (auto itr = m_Stack.rbegin(); itr != m_Stack.rend(); itr++) {
        if (!(*itr)->handleEvent(event))
            break;
    }
    applyPendingChanges();
}
void StateStack::pushState(Inkball::States::Id stateID)
{

     m_PendingList.push_back(PendingChange(Action::Push, stateID));
    
}
void StateStack::pushState(State* ptr)
{
    m_PendingList.push_back(PendingChange(Action::Push_Custom, ptr));
}
void StateStack::popState()
{
    m_PendingList.push_back(PendingChange(Action::Pop));
}

void StateStack::clearStates()
{
    m_PendingList.push_back(PendingChange(Action::Clear));
}

bool StateStack::isEmpty() const
{
    return m_Stack.size() == 0;
}

State::Ptr StateStack::createState(Inkball::States::Id stateID)
{
    auto found = m_Factories.find(stateID);
    assert(found != m_Factories.end());

    return found->second();
}

void StateStack::applyPendingChanges()
{
    for (const auto& change : m_PendingList) {

        switch (change.s_action) {

        case Action::Push:
         m_Stack.push_back(createState(change.s_stateID));
        break;
        case Action::Push_Custom:
            //nooo
            m_Stack.emplace_back(change.s_inject);
            
         break;
        case Action::Pop:
         m_Stack.pop_back();
        break;
        case Action::Clear:
         m_Stack.clear();
        break;
        }
    }
    m_PendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action,Inkball::States::Id stateID):s_action(action),s_stateID(stateID)
{
}

StateStack::PendingChange::PendingChange(Action action, State* ptr):s_action(action),s_inject(ptr)
{
}
