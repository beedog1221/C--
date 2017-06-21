#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
public class FSM {
    private var activeState :Function; 
 
    public function FSM() {
    }
 
    public function setState(state :Function) :void {
        activeState = state;
    }
 
    public function update() :void {
        if (activeState != null) {
            activeState();
        }
    }
}
#endif