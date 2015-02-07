#ifndef __TIMEWATCHER_H__
#define __TIMEWATCHER_H__

#include <string>
#include <vector>
#include <iostream>

class Watch
{
    protected:
        std::string _name;
        std::string _subname;
    public:
        Watch(std::string name, std::string subname):
            _name(name),_subname(subname)
        {
        }
        
        inline const std::string& name() const
        {
            return _name;
        }
        
        inline const std::string& subname() const
        {
            return _subname;
        }
        
        virtual void run() = 0;
        
        virtual ~Watch()
        {
        }
};


#define WATCH(NAME,SUBNAME) \
    class Watch ## NAME ## SUBNAME : public Watch { \
    public: \
    Watch ## NAME ## SUBNAME (): Watch( #NAME , #SUBNAME ) { \
        TimeWatcher::getInstance().registerWatch( this ); \
    } \
    virtual void run(); \
    }; \
    static Watch ## NAME ## SUBNAME  Watch ## NAME ## SUBNAME ; \
    void Watch ## NAME ## SUBNAME ::run() 
    
       

class TimeWatcher
{
    protected:
        std::vector<Watch*> _watches;
        
        TimeWatcher()
        {
        }
    public:
        static TimeWatcher& getInstance()
        {   
            static TimeWatcher timeWatcher;
            return timeWatcher;
        }
        
        void registerWatch(Watch* watch)
        {
            
            _watches.push_back(watch);
        }
        
        unsigned int size() const
        {
            return _watches.size();
        }
        
        void runAll()
        {
            for (unsigned int i = 0; i < _watches.size(); ++i)
            {
                std::cout<<"Running: "<<_watches[i]->name()<<"."<<_watches[i]->subname()<<" ..."<<std::endl;
                std::cout<<"--------------------------------"<<std::endl;
                _watches[i]->run();
                std::cout<<"--------------------------------"<<std::endl;
                std::cout<<"... done"<<std::endl;
                std::cout<<std::endl;
            }
        }
};



#endif

