#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

#define dateWindows 1

class Time_
{
private:
    int hour;
    int minutes;
    int seconds;
    bool format; // true = 24-hours, false = 12-hours (AM/PM)

public:
    Time_() : format(true) {
        tm info = {};
        auto current = time(0);
#if dateWindows == 1
        localtime_s(&info, &current);
#elif dateWindows == 0
        tm* p = localtime(&current);
        info = *p;
#endif
        hour = info.tm_hour;
        minutes = info.tm_min;
        seconds = info.tm_sec;
    }

    Time_(int hour, int minutes, int seconds, bool format = true)
        : hour(hour), minutes(minutes), seconds(seconds), format(format) {
        if (!valid()) {
            this->hour = 0;
            this->minutes = 0;
            this->seconds = 0;
        }
    }

    Time_(const Time_& obj) = default;
    Time_& operator=(const Time_& obj) = default;
    ~Time_() = default;

    void setHour(int hour) { if (hour >= 0 && hour < 24) this->hour = hour; }
    int getHour() const { return hour; }

    void setMinutes(int minutes) { if (minutes >= 0 && minutes < 60) this->minutes = minutes; }
    int getMinutes() const { return minutes; }

    void setSeconds(int seconds) { if (seconds >= 0 && seconds < 60) this->seconds = seconds; }
    int getSeconds() const { return seconds; }

    void setFormat(bool format) { this->format = format; }
    bool getFormat() const { return format; }

    bool valid() const {
        return (hour >= 0 && hour < 24) &&
            (minutes >= 0 && minutes < 60) &&
            (seconds >= 0 && seconds < 60);
    }

    void tickTime() {
        *this += 1.0f;
    }

    void untickTime() {
        *this -= 1.0f;
    }

    void showTime() const {
        if (format) {
            cout << setfill('0') << setw(2) << hour << ":"
                << setw(2) << minutes << ":"
                << setw(2) << seconds;
        }
        else {
            int displayHour = hour % 12;
            if (displayHour == 0) displayHour = 12;
            string period = (hour >= 12) ? "PM" : "AM";

            cout << setfill('0') << setw(2) << displayHour << ":"
                << setw(2) << minutes << ":"
                << setw(2) << seconds << " " << period;
        }
    }

    long toSeconds() const {
        return hour * 3600 + minutes * 60 + seconds;
    }

    void fromSeconds(long totalSec) {
        totalSec %= (24 * 3600);
        if (totalSec < 0) totalSec += (24 * 3600);

        hour = totalSec / 3600;
        totalSec %= 3600;
        minutes = totalSec / 60;
        seconds = totalSec % 60;
    }

    // --------- Comparison operators ---------
    bool operator==(const Time_& obj) const& { return toSeconds() == obj.toSeconds(); }
    bool operator!=(const Time_& obj) const& { return !(*this == obj); }
    bool operator>(const Time_& obj) const& { return toSeconds() > obj.toSeconds(); }
    bool operator<(const Time_& obj) const& { return toSeconds() < obj.toSeconds(); }
    bool operator>=(const Time_& obj) const& { return !(*this < obj); }
    bool operator<=(const Time_& obj) const& { return !(*this > obj); }

    // --------- Assignment operators ---------
    Time_& operator+=(float s) {
        fromSeconds(toSeconds() + static_cast<long>(s));
        return *this;
    }
    Time_& operator-=(float s) {
        fromSeconds(toSeconds() - static_cast<long>(s));
        return *this;
    }

    Time_& operator+=(int m) {
        fromSeconds(toSeconds() + m * 60);
        return *this;
    }
    Time_& operator-=(int m) {
        fromSeconds(toSeconds() - m * 60);
        return *this;
    }

    Time_& operator+=(long h) {
        fromSeconds(toSeconds() + h * 3600);
        return *this;
    }
    Time_& operator-=(long h) {
        fromSeconds(toSeconds() - h * 3600);
        return *this;
    }

    // --------- Arithmetic operators ---------
    Time_ operator+(float s) const& {
        Time_ temp = *this;
        temp += s;
        return temp;
    }
    Time_ operator-(float s) const& {
        Time_ temp = *this;
        temp -= s;
        return temp;
    }

    Time_ operator+(int m) const& {
        Time_ temp = *this;
        temp += m;
        return temp;
    }
    Time_ operator-(int m) const& {
        Time_ temp = *this;
        temp -= m;
        return temp;
    }

    Time_ operator+(long h) const& {
        Time_ temp = *this;
        temp += h;
        return temp;
    }
    Time_ operator-(long h) const& {
        Time_ temp = *this;
        temp -= h;
        return temp;
    }
};

class Schedule {
private:
    Time_ startTime;
    int lessonDuration;
    int shortBreak;
    int longBreak;
    int longBreakAfter;
    int totalLessons;

public:
    Schedule(Time_ start, int lessonDur, int shortBr, int longBr, int longAfter, int lessons)
        : startTime(start), lessonDuration(lessonDur), shortBreak(shortBr),
        longBreak(longBr), longBreakAfter(longAfter), totalLessons(lessons) {
    }

    void generateSchedule() const {
        cout << "\n================ LESSON SCHEDULE ================\n";
        Time_ currentStart = startTime;

        for (int i = 1; i <= totalLessons; ++i) {
            Time_ currentEnd = currentStart + lessonDuration;

            cout << "Lesson " << i << ": ";
            currentStart.showTime();
            cout << " - ";
            currentEnd.showTime();
            cout << endl;

            if (i < totalLessons) {
                int currentBreak = (i == longBreakAfter) ? longBreak : shortBreak;
                currentStart = currentEnd + currentBreak;
            }
        }
        cout << "=================================================\n";
    }
};

int main() {
    cout << "--- Task 1: Time_ class demonstration ---\n";
    Time_ now;
    cout << "Current local time (24h): ";
    now.showTime();
    cout << endl;

    now.setFormat(false);
    cout << "Current local time (12h AM/PM): ";
    now.showTime();
    cout << "\n\n";

    Time_ t1(8, 30, 0);
    Time_ t2(10, 15, 30);

    cout << "t1: "; t1.showTime(); cout << endl;
    cout << "t2: "; t2.showTime(); cout << endl;

    cout << "\nComparison operations:\n";
    cout << "t1 == t2: " << (t1 == t2 ? "True" : "False") << endl;
    cout << "t1 != t2: " << (t1 != t2 ? "True" : "False") << endl;
    cout << "t1 < t2:  " << (t1 < t2 ? "True" : "False") << endl;
    cout << "t1 > t2:  " << (t1 > t2 ? "True" : "False") << endl;
    cout << "t1 <= t2: " << (t1 <= t2 ? "True" : "False") << endl;
    cout << "t1 >= t2: " << (t1 >= t2 ? "True" : "False") << endl;

    cout << "\nArithmetic operations (t1 = 08:30:00):\n";
    Time_ t3 = t1 + 45;
    cout << "t1 + 45 min: "; t3.showTime(); cout << endl;

    t3 = t1 - 15;
    cout << "t1 - 15 min: "; t3.showTime(); cout << endl;

    t3 = t1 + 2L;
    cout << "t1 + 2 hours: "; t3.showTime(); cout << endl;

    t3 = t1 - 1L;
    cout << "t1 - 1 hour: "; t3.showTime(); cout << endl;

    t3 = t1 + 30.0f;
    cout << "t1 + 30 sec: "; t3.showTime(); cout << endl;

    t3 = t1 - 10.0f;
    cout << "t1 - 10 sec: "; t3.showTime(); cout << endl;

    t1.tickTime();
    cout << "t1 after tickTime (+1 sec): "; t1.showTime(); cout << endl;

    t1.untickTime();
    cout << "t1 after untickTime (-1 sec): "; t1.showTime(); cout << endl;

    cout << "\nAssignment operations:\n";
    t1 += 15;
    cout << "t1 += 15 min: "; t1.showTime(); cout << endl;

    t1 -= 15;
    cout << "t1 -= 15 min: "; t1.showTime(); cout << endl;

    cout << "\n--- Task 2: Schedule generation ---\n";
    Time_ startSchool(8, 30, 0);
    Schedule todaySchedule(startSchool, 45, 10, 20, 2, 5);
    todaySchedule.generateSchedule();

    return 0;
}