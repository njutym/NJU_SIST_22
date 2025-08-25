#include<iostream>
using namespace std; 
 
// TODO: implement the following class
//       you can add any pulic interfaces if needed, 
//       but do not expose any data member public

class Date {
    friend class Time;
    friend class ExtTime;
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
      if(month == 2)
        {
          if(leap)
          {
            if(day == 29)
            {
            day = 1;
            month++;
            }
            else day++;
          }
          else
          {
            if(day == 28)
            {
            day = 1;
            month++;
            }
            else day++;
          }
        }
      else
      {
        if(day = days[month-1])
        {
          day = 1;
          month++;
        }
        else day++;
      }
      if(month > 12)
      {
        month = 1;
        year++;
      }
      /*if(leap) days[1]++;
      if(day == days[month-1])
      {
        day = 1;
        month++;
      }
      else day++;
      if(month > 12)
      {
        year++;
        month = 1;
      }
    }*/
 
};

class Time {
  friend class ExtTime;
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
      return (dates.year == other_time.dates.year && dates.month == other_time.dates.month && dates.day == other_time.dates.day
          && hour == other_time.hour && minute == other_time.minute && second == other_time.second);
    }
    bool less_than(Time &other_time)
    {
      if(dates.year < other_time.dates.year) return true;
      if(dates.year > other_time.dates.year) return false;
      else
      {
        if(dates.month < other_time.dates.month) return true;
        if(dates.month > other_time.dates.month) return false;
        else
        {
          if(dates.day < other_time.dates.day) return true;
          if(dates.day > other_time.dates.day) return false;
          else
          {
            if(hour < other_time.hour) return true;
            if(hour > other_time.hour) return false;
            else
            {
              if(minute < other_time.minute) return true;
              if(minute > other_time.minute) return false;
              else
              {
                if(second < other_time.second) return true;
                if(second >= other_time.second) return false;
              }
            }
          }
        }
      }
    }
};

class ExtTime {
  private:
    Time time;
    int zones;
  public:
    ExtTime():zones(1),time(){}
    ExtTime(int zone, Date & date, int h, int m, int s):time(date,h,m,s),zones(zone){}
    void set(int zone, Date & date, int h, int m, int s)
    {
      zones = zone;
      time.dates.year = date.year;
      time.dates.month = date.month;
      time.dates.day = date.day;
      time.hour = h;
      time.minute = m;
      time.second = s;
    }
    void display()
    {
      cout<<"第"<<zones<<"时区";
      time.display();
    }
    void increment()
    {
    	time.increment();
	  }
    bool equal(ExtTime &other_times)
    {
      return zones == other_times.zones && time.equal(other_times.time);
    }
    bool less_than(ExtTime &other_times)
    {
      if(zones < other_times.zones) return true;
      return false;
    }
};
