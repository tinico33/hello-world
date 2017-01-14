#define TIME_SHORT_PUSH 200
#define TIME_SHORT_ACTION 15000

class Shutter {
  public :
    Shutter();
    void attach(char relayUp, char relayDown, char buttonUp, char buttonDown);
    void action();
    void openCompletly();
    void closeCompletly();
    void stop();
  private :
    void open();
    void close();
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
    bool inAction;
};

