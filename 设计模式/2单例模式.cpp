class Singleton
{
public:
    static Singleton& Instance(){
        //lock();C++11之后不需要
        static Singleton singleton;
        //unlock();
        return singleton;
    }
private:
    Singleton(){};
    ~Singleton();
}