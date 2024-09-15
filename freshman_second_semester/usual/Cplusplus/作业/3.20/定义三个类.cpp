#include<iostream>
using namespace std; 
 
// TODO: implement the following class
//       you can add any pulic interfaces if needed, 
//       but do not expose any data member public

class Date {
  private:
    int year;
    int month;
    int day;
  public:
    Date():year(2222),month(2),day(22){}
    Date(int y, int m, int d):year(y),month(m),day(d){}
    void set(int y, int m, int d)
    {
      year =  y;
      month = m;
      day = d;
    }
    void display()
    {
      cout<<year<<"年"<<month<<"月"<<day<<"日"<<endl;
    }
    void increase()
    {
      int leap = 0;
      if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) leap = 1;
      int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
      if(leap == 0) days[1]++;
      if(day == days[month-1])
      {
        day = 1;
        month++;
      }
      else day++;
      if(month > 12)
      {
        year++;
        month -= 12;
      }
    }
    friend class Time;
    friend class Exttime;
};

class Time {
  private:
    int hour;
    int minute;
    int second;
    Date dates;
  public:
    Time():dates(),hour(2),minute(2),second(2){}
    Time(Date & date, int h, int m, int s):dates(date),hour(h),minute(m),second(s){}
    void set(Date & date, int h, int m, int s)
    {
      dates = date;
      hour = h;
      minute = m;
      second = s;
    }
    void display()
    {
      cout<<dates.year<<"年"<<dates.month<<"月"<<dates.day<<"日"<<hour<<"点"<<minute<<"分"<<second<<"秒"<<endl;
    }
    void increment()
    {
      second++;
      if(second == 60)
      {
        minute++;
        second = 0;
        if(minute == 60)
        {
          hour++;
          minute = 0;
          if(hour == 24)
          {
            dates.increase();
            hour = 0;
           }
        }
      }
    }
    bool equal(Time &other_time)
    {
      return (dates == other_time.dates);
    }
    bool less_than(Time &);
};

class ExtTime {
  public:
    ExtTime();
    ExtTime(int zone, Date & date, int h, int m, int s);
    void set(int zone, Date & date, int h, int m, int s);
    void display();
    void increment();
    bool equal(ExtTime &);
    bool less_than(ExtTime &);
};
