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
      if(leap) days[1]++;
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
    }
 
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
      time.dates = date;
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
      return (zones == other_times.zones) && time.equal(other_times.time);
    }
    bool less_than(ExtTime &other_time)
    {
      if(zones < other_time.zones) return true;
      return false;
    }
};






int main() {
    // Testing class Date
    Date date1(2022, 6, 30);
    date1.display(); // Output: 2022年6月30日
    date1.set(2023, 7, 31);
    date1.display(); // Output: 2023年7月31日
    Date date2;
    date2.display(); // Output: 2222年2月22日

    // Testing class Time
    Date date3(2022, 2, 28);
    Time time1(date3, 23, 59, 59);
    time1.display(); // Output: 2022年2月28日23点59分59秒
    time1.increment(); // Add one second
    time1.display(); // Output: 2022年3月1日0点0分0秒
    Time time2;
    time2.display(); // Output: 2222年2月22日2点2分2秒

    // Testing class ExtTime
    Date date4(2022, 6, 30);
    ExtTime extTime1(2, date4, 23, 59, 59);
    extTime1.increment();
    extTime1.display(); // Output: 第2时区2022年6月30日23点59分59秒
    ExtTime extTime2(3, date4, 23, 59, 59);
    extTime2.display(); // Output: 第3时区2022年6月30日23点59分59秒
    cout << extTime1.less_than(extTime2) << endl; // Output: 1 (true)
    cout << extTime2.less_than(extTime1) << endl; // Output: 0 (false)
    ExtTime extTime3(2, date4, 0, 0, 0);
    extTime3.display(); // Output: 第2时区2022年6月30日0点0分0秒
    cout << extTime1.equal(extTime3) << endl; // Output: 0 (false)
    cout << extTime1.less_than(extTime3) << endl; // Output: 0 (false)

    return 0;
}
