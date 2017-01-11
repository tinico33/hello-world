#define TIME_SHORT_ACTION 15000 // the shortest pulse sent to a servo  

class Volet {
  public :
    Volet();
    void attach(char relayUp, char relayDown, char voletUp, char voletDown);
    void action();
  private :
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
    void open();
    void close();
    void stop();
};
