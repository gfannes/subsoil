#include "Arduino.h"
#include "Servo.h"

namespace  { 
    class Clock
    {
    public:
        void init()
        {
            prev_time_ =  millis();
        }
        unsigned int elapse()
        {
            const auto new_time = millis();
            const unsigned int ret = new_time - prev_time_;
            prev_time_ = new_time;
            return ret;
        }
    private:
        unsigned int prev_time_;
    };

    class Timer
    {
    public:
        void reset(unsigned int ms) {orig_ = ms_ = ms;}

        bool operator()(unsigned int &ms)
        {
            if (ms >= ms_)
            {
                ms -= ms_;
                ms_ = 0;
                return true;
            }
            ms_ -= ms;
            ms = 0;
            return false;
        }

        float frac() const
        {
            return float(orig_-ms_)/float(orig_);
        }

    private:
        unsigned int orig_ = 0;
        unsigned int ms_ = 0;
    };

    enum class Side {Left, Right};
    inline Side other(Side side) {return (side == Side::Left ? Side::Right : Side::Left);}

    class Oar
    {
    public:
        void init()
        {
            yaw_.attach(6);
            roll_.attach(7);
        }
        void up()
        {
            yaw_.write(90);
            roll_.write(90);
        }
        void pre(Side side)
        {
            roll_.write(90);
            yaw_.write(pre_(side));
        }
        void row(Side side, float frac)
        {
            {
                int angle = 85;
                angle += (side == Side::Left ? -25 : 25);
                roll_.write(angle);
            }
            {
                const auto begin = pre_(side);
                const auto end = pre_(other(side));
                const int angle = begin + frac*(end-begin);
                yaw_.write(angle);
            }
        }

    private:
        int pre_(Side side) const
        {
            int angle = 90;
            angle += (side == Side::Left ? -10 : 10);
            return angle;
        }

        Servo yaw_;
        Servo roll_;
    };

    class StateMachine
    {
    private:
        enum class State {_, Init, Up, PreLeft, Left, PreRight, Right};

    public:
        void process(unsigned int &elapse)
        {
            switch (state_)
            {
                case State::_:
                    return change_state_(State::Init);
                    break;

                case State::Up:
                    if (timer_(elapse))
                        return change_state_(State::PreLeft);
                    break;

                case State::PreLeft:
                    if (timer_(elapse))
                        return change_state_(State::Left);
                    break;
                case State::PreRight:
                    if (timer_(elapse))
                        return change_state_(State::Right);
                    break;

                case State::Left:
                    if (timer_(elapse))
                        return change_state_(State::PreRight);
                    oar_.row(Side::Left, timer_.frac());
                    break;
                case State::Right:
                    if (timer_(elapse))
                        return change_state_(State::PreLeft);
                    oar_.row(Side::Right, timer_.frac());
                    break;
            }
        }

    private:
        void change_state_(State wanted_state)
        {
            if (state_ == wanted_state)
                return;
            //Exit actions
            switch (state_)
            {
                default: break;
            }

            state_ = wanted_state;

            //Enter actions
            switch (state_)
            {
                case State::Init:
                    oar_.init();
                    change_state_(State::Up);
                    break;
                case State::Up:
                    oar_.up();
                    timer_.reset(20);
                    break;

                case State::PreLeft:
                    oar_.pre(Side::Left);
                    timer_.reset(100);
                    break;
                case State::PreRight:
                    oar_.pre(Side::Right);
                    timer_.reset(100);
                    break;

                case State::Left:
                    timer_.reset(500);
                    break;
                case State::Right:
                    timer_.reset(500);
                    break;
                default: break;
            }
        }
        State state_ = State::_;

        Oar oar_;
        Timer timer_;
    };

    StateMachine sm;
    Clock clock;
} 

void setup() {
    clock.init();
}

void loop() {
    auto elapse = clock.elapse();

    do {
        sm.process(elapse);
    } while (elapse != 0);
}
