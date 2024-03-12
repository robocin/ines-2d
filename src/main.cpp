#include <iostream>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/event.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/statechart/state.hpp>
#include <thread>
#include <unistd.h>

namespace sc = boost::statechart;

struct Transition : sc::event<Transition> {};

struct Kicker;
struct updateWorldModel;
struct goToBall;
struct shoot;
struct dribble;

struct Kicker : sc::state_machine<Kicker, updateWorldModel> {
    public:
        Kicker() : worldModel(1), kickable(1), canShoot(true), updatedWorldModel(true) {};
        ~Kicker() {};

        int getWorldModel() const { return worldModel; }
        int &getWorldModel() { return worldModel; }
        int getKickable() const { return kickable; }
        int &getKickable() {return kickable; }
        bool getCanShoot() const {return canShoot;}
        bool &getCanShoot() {return canShoot; } 
        bool getUpdatedWorldModel() const {return updatedWorldModel;}
        bool &getUpdatedWorldModel() {return updatedWorldModel; } 

    private:
        int worldModel;  
        int kickable;
        bool canShoot;
        bool updatedWorldModel;
};

struct updateWorldModel: sc::state<updateWorldModel, Kicker> {
    public:
        typedef sc::custom_reaction<Transition> reactions;

        updateWorldModel(my_context ctx) : my_base(ctx) {
            std::cout << "Updating the world model!\n";
        }

        ~updateWorldModel() {
            std::cout << "World model updated\n";
        }

        sc::result react(const Transition&) {
            if(!context<Kicker>().getUpdatedWorldModel()) {
                return transit<updateWorldModel>();
            }else{
                return transit<goToBall>();
            }
        }
};

struct goToBall: sc::state<goToBall, Kicker> {
    public:
        typedef sc::custom_reaction<Transition> reactions;

        goToBall(my_context ctx) : my_base(ctx) {
            std::cout << "Going to ball position!\n";
        }

        ~goToBall() {
            std::cout << "Quitting go to ball\n";
        }

        sc::result react(const Transition&) {
            if(context<Kicker>().getKickable() == 1) {
                return transit<shoot>();
            }else{
                return transit<updateWorldModel>();
            }
        }
};

struct shoot: sc::state<shoot, Kicker> {
        typedef sc::custom_reaction<Transition> reactions;

        shoot(my_context ctx) : my_base(ctx) {
            std::cout << "Entering shoot!\n";
        }

        ~shoot() {
            std::cout << "Quitting shoot\n";
        }

        sc::result react(const Transition&) {
            if(context<Kicker>().getCanShoot()) {
                return transit<updateWorldModel>();
            }else{
                return transit<dribble>();
            }
        }
};

struct dribble: sc::state<dribble, Kicker> {
        typedef sc::custom_reaction<Transition> reactions;

        dribble(my_context ctx) : my_base(ctx) {
            std::cout << "Entering dribble!\n";
        }

        ~dribble() {
            std::cout << "Quitting dribble\n";
        }

        sc::result react(const Transition&) {
            return transit<updateWorldModel>();
        }
};

void trigger(Kicker &machine) {
    for(int i = 0;i < 20;i++) {
        machine.process_event(Transition());
    }
}

int main() {
    Kicker machine;
    machine.initiate();
    std::thread triggerEvent(trigger, std::ref(machine));
    triggerEvent.join();
    return 0;
}