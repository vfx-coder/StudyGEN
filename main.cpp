#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Subject {
    string name;
    int difficulty;
    int daysLeft;
    int completion;
    int priority;
};

// ---------- Difficulty Label ----------
string difficultyLabel(int d) {
    if (d <= 2) return "Easy";
    if (d == 3) return "Medium";
    return "Hard";
}

// ---------- Sort by priority ----------
bool comparePriority(const Subject &a, const Subject &b) {
    return a.priority > b.priority;
}

int main() {

    int choice, n;
    int collegeStart, collegeEnd;
    int chillTime, sleepTime;

    cout << "===== Welcome to StudySync =====\n";
    cout << "1. Daily Based Planning\n";
    cout << "2. Exam Based Planning (Auto Scheduler)\n";
    cout << "Enter choice: ";
    cin >> choice;

    // ===== DAILY ROUTINE =====
    cout << "\n--- Daily Routine Setup ---\n";
    cout << "College start hour (0-23): ";
    cin >> collegeStart;

    cout << "College end hour (0-23): ";
    cin >> collegeEnd;

    cout << "Daily chill/personal time (hours): ";
    cin >> chillTime;

    cout << "Daily sleep time (hours): ";
    cin >> sleepTime;

    int collegeHours = collegeEnd - collegeStart;
    if (collegeHours < 0) collegeHours += 24;

    int totalStudyHours =
        24 - collegeHours - chillTime - sleepTime;

    if (totalStudyHours <= 0) {
        cout << "\nNo available study hours! "
             << "Adjust chill/sleep time.\n";
        return 0;
    }

    cout << "\nAuto Calculated Study Hours per Day: "
         << totalStudyHours << " hrs\n";

    // ===== SUBJECT INPUT =====
    cout << "\nEnter number of subjects: ";
    cin >> n;

    vector<Subject> data(n);

    for (int i = 0; i < n; i++) {

        cout << "\nSubject " << i + 1 << " name: ";
        cin >> data[i].name;

        cout << "Difficulty (1-5): ";
        cin >> data[i].difficulty;

        cout << "Completion percentage (0-100): ";
        cin >> data[i].completion;

        if (choice == 2) {
            cout << "Days left for exam: ";
            cin >> data[i].daysLeft;
        } else {
            data[i].daysLeft = 0;
        }
    }

    // ================= DAILY MODE =================
    if (choice == 1) {

        for (auto &s : data) {
            s.priority =
                s.difficulty * 2 +
                (100 - s.completion) / 10;
        }

        sort(data.begin(), data.end(), comparePriority);

        cout << "\n===== Priority List =====\n";
        for (auto &s : data) {
            cout << s.name << " (" << difficultyLabel(s.difficulty)
                 << ") -> Priority: " << s.priority << endl;
        }

        cout << "\n===== Today's Study Plan =====\n";

        int remainingHours = totalStudyHours;

        for (auto &s : data) {

            if (remainingHours <= 0) break;

            int allocated = min(2, remainingHours);

            cout << "- " << s.name
                 << " : " << allocated << " hrs\n";

            remainingHours -= allocated;
        }
    }

    // ================= EXAM MODE =================
    else {

        cout << "\n===== Auto Daily Schedule Until Exam =====\n";

        int day = 1;
        bool allDone = false;

        while (!allDone) {

            allDone = true;

            for (auto &s : data) {
                if (s.daysLeft > 0 && s.completion < 100)
                    allDone = false;
            }

            if (allDone) break;

            // Calculate priorities daily
            for (auto &s : data) {

                if (s.daysLeft > 0 && s.completion < 100) {

                    int urgency = max(0, 10 - s.daysLeft);

                    s.priority =
                        s.difficulty * 2 +
                        urgency +
                        (100 - s.completion) / 10;
                }
                else {
                    s.priority = -1;
                }
            }

            sort(data.begin(), data.end(), comparePriority);

            cout << "\n--- Day " << day << " Plan ---\n";

            cout << "Priority Order:\n";
            for (auto &s : data) {
                if (s.priority >= 0)
                    cout << s.name << " -> "
                         << s.priority << endl;
            }

            int remainingHours = totalStudyHours;

            for (auto &s : data) {

                if (remainingHours <= 0) break;
                if (s.daysLeft <= 0 || s.completion >= 100)
                    continue;

                int allocated = min(2, remainingHours);

                // 1 hour = +10% completion
                s.completion += allocated * 10;
                if (s.completion > 100) s.completion = 100;

                cout << "- " << s.name
                     << " : " << allocated << " hrs"
                     << " | Completion: "
                     << s.completion << "%\n";

                remainingHours -= allocated;
            }

            // Progress Summary
            cout << "\nProgress Summary:\n";
            for (auto &s : data) {
                cout << s.name << " : "
                     << s.completion << "% complete\n";
            }

            // Day passes
            for (auto &s : data) {
                if (s.daysLeft > 0)
                    s.daysLeft--;
            }

            day++;
        }

        cout << "\nAll plans generated until exams or completion reached!\n";
    }

    return 0;
}
