#ifndef __TIMEWATCHER_H__
#define __TIMEWATCHER_H__

#include <string>
#include <vector>
#include <iostream>
#include <thread>
#include <functional>
#include <chrono>

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

        virtual std::function<void(long)> getFunction() = 0;
        
        virtual ~Watch()
        {
        }
};


#define WATCH(NAME,SUBNAME) \
    class Watch ## NAME ## SUBNAME : public Watch { \
        std::function<void(long)> _fct; \
    public: \
    Watch ## NAME ## SUBNAME (): Watch( #NAME , #SUBNAME ) { \
        TimeWatcher::getInstance().registerWatch( this ); \
        _fct=std::bind(& Watch ## NAME ## SUBNAME ::run, std::ref(*this), std::placeholders::_1); \
    } \
    void run(long iteration) const; \
    virtual std::function<void(long)> getFunction() { return _fct; } \
    }; \
    static Watch ## NAME ## SUBNAME  Watch ## NAME ## SUBNAME ; \
    void Watch ## NAME ## SUBNAME ::run(long iteration) const

#define INTERATION iteration
    

class Measurement
{
    private:
        std::function<void(long)> _fct;
        std::vector<long> _counts;
        long _ms;

        void execute(std::function<void(long)> fct, bool& stop, long& done)
        {
            long iteration = 0;
            while (!stop)
            {
                fct(iteration);
                ++iteration;
            }
            done = iteration;
        }
    public:
        Measurement(std::function<void(long)> fct, long ms=250, unsigned int repeat=0):
            _fct(fct), _counts(repeat+1), _ms(ms)
        {
        }

        void run()
        {
            for (unsigned int irun = 0; irun < _counts.size(); ++irun)
            {
                bool stop = false;
                long count = 0;
                std::function<void(void)> exec = std::bind(&Measurement::execute,std::ref(*this),_fct,std::ref(stop),std::ref(count));
                std::thread th(exec);
                std::this_thread::sleep_for(std::chrono::milliseconds(_ms));
                stop=true;
                th.join();
                _counts[irun]=count;
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        double getAverageMS() const
        {
            double average = 0.0;
            for (unsigned int irun = 0; irun < _counts.size(); ++irun)
            {
                average+=(1.0*_ms)/_counts[irun];
            }
            average/=_counts.size();
            return average;
        }


};



class TimeWatcher
{
    protected:
        std::vector<Watch*> _watches;
        
        TimeWatcher()
        {
        }

        std::string formatTime(double& mstime)
        {
            std::vector<std::string> names = {"ps","ns","microns","ms","s"};
            unsigned int i = 2;
            while (mstime>1000.0 && i<(names.size()-1))
            {
                mstime/=1000.0;
                ++i;
            }
            while (mstime<0.001 && i>0)
            {
                mstime*=1000.0;
                --i;
            }
            return names[i];
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
                std::cout<<"Running: "<<_watches[i]->name()<<"."<<_watches[i]->subname()<<" ... : ";
                Measurement measurement(_watches[i]->getFunction(),200,0);
                measurement.run();
                double time = measurement.getAverageMS();
                std::string timeName = formatTime(time);
                printf("%4.3e %s/call\n",time,timeName.c_str());
            }
        }
};



#endif

