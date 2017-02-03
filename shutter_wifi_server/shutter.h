#define TIME_SHORT_PUSH 200
#define TIME_SHORT_ACTION 15000
#define CONNECTION_TEST_TIMER 10000

class Shutter {
  public :
    Shutter();
    void attach(char relayUp, char relayDown, char buttonUp, char buttonDown);
    void action();
    int openCompletly();
    int closeCompletly();
    void stop(bool timeout);
    int getState();
  private :
    void open();
    void close();
    bool inAction();
    void setTimeStop();
    char relayUp;
    char relayDown;
    char buttonUp;
    char buttonDown;
    int buttonDownValue;
    int buttonUpValue;
    int buttonDownMemory;
    int buttonUpMemory;
    unsigned long timeUp;
    unsigned long timeDown;
    unsigned long timeStop;
    enum State{GOING_UP, OPENED, GOING_DOWN, CLOSED, STOPED};
    State state;
};

