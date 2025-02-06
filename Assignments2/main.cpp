#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <numeric>
#include <string>

using namespace std;

class StudentScoreManager
{
private:
	// �л� ������ ���� ��
	// key = �л� ��ȣ, value = �ش� �л��� ����
	map<int, map<string, int>> Students;

	// ������� �߰��� ���� ���
	set<string> Subjects;

public:
	// �л� ���� �߰�
	void AddStudentScore(int Id, string Subject, int Score)
	{
		cout << endl;

		// ���� �߰�
		Subjects.insert(Subject);

		// ���� �ֽ�ȭ
		Students[Id][Subject] = Score;

		cout << Id << "�� �л��� \'" << Subject << "\'�� ������ " << Score << "(��)�� �ֽ�ȭ �Ǿ����ϴ�." << endl;

		cout << endl;
	}

	// Ư�� �л��� ��� ���� ���� ���
	void PrintAllScoreByStudentId(int Id)
	{
		cout << endl;

		// �Է��� ��ȣ�� �л��� ���� ���
		if (Students.find(Id) == Students.end())
		{
			cout << Id << "�� �л��� ������ �����ϴ�." << endl;
			cout << endl;
			return;
		}

		// Ư�� �л��� ��� ���� ����
		map<string, int> Scores{ Students[Id] };

		cout << Id << "�� �л��� ����: " << endl;

		for (auto& [Subject, Score] : Scores)
		{
			cout << "- " << Subject << ": " << Score << "��" << endl;
		}

		cout << endl;
	}

	// �� ���� ��ü �л��� ��� ���� ���
	void PrintAllSubjectAverageScore()
	{
		cout << endl;
		// ���� ���� ��Ƽ ��
		multimap<string, int> AllScores;

		// ��ü �л� ��ȸ
		for (auto& [Id, Scores] : Students)
		{
			// �� �л��� ����
			for (auto& [Subject, Score] : Scores)
			{
				// ���� ���� �߰�
				AllScores.insert({ Subject, Score });
			}
		}

		cout << "��ü ���� ��� ����: " << endl;

		for (string Subject : Subjects)
		{
			// ������ Subject�� ��� ���� ��ȸ
			auto Range = AllScores.equal_range(Subject);
			// �ش� �������� ��
			int Sum = 0;
			for (auto& It = Range.first; It != Range.second; It++)
			{
				Sum += It->second;
			}
			// �ش� �������� ����
			int Count = AllScores.count(Subject);

			// �ش� ������ ��ü ���
			double Avg = (double)Sum / (double)Count;

			// �Ҽ��� ��° �ڸ����� ǥ���ϱ� ���� ���
			Avg *= 100;
			Avg = round(Avg);
			Avg /= 100;

			cout << "- " << Subject << ": " << Avg << "��" << endl;
		}

		cout << endl;
	}

	// �ش� ������ ����� �������� Ȯ��(�ش� ������ ������ �л��� �ִ��� Ȯ��)
	bool IsTakingSubject(string Subject)
	{
		// ����� ������ �ƴ� ���(�ش� ������ ������ �л��� ���� ���)
		if (Subjects.find(Subject) == Subjects.end())
		{
			cout << "\n�ش� ������ ������ �л��� �����ϴ�." << endl;
			cout << endl;
			return false;
		}

		return true;
	}

	// Ư�� ������ �����ϴ� �л� ���
	// first = �л� ��ȣ, second = �ش� ���� ����
	// �⺻ �л� ��ȣ ���� �������� ����
	vector<pair<int, int>> GetStudentsBySubject(string Subject)
	{
		// �ش� ������ �����ϴ� �л��� ��ȣ
		vector<pair<int, int>> StudentScores;

		// ��ü �л� ��ȸ
		for (auto& [Id, Scores] : Students)
		{
			// �ش� ������ �����ϴ� �л��� ���
			if (Scores.find(Subject) != Scores.end())
			{
				StudentScores.push_back({Id, Scores[Subject]});
			}
		}

		// ������������ ����
		sort(StudentScores.begin(), StudentScores.end(),
			[](pair<int, int> A, pair<int, int> B)
			{
				return A.first < B.first;
			});

		return StudentScores;
	}

	// Ư�� ���񿡼� ���� ���� ������ ���� �л��� ���
	void PrintTopStudentsBySubejct(string Subject)
	{
		cout << endl;

		// �ش� ������ �����ϴ� �л��� ��ȣ
		vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);
		
		// �ְ� ���� �л����� ��ȣ
		vector<int> StudentIDs;

		// �ְ� ����
		int TopScore = 0;

		// ��ü �л� ��ȸ
		for (auto& [Id, Score] : StudentScores)
		{
			// ���� ������ ���� �ְ� �������� Ŭ ���
			if (Score > TopScore)
			{
				StudentIDs.clear();
				StudentIDs.push_back(Id);
				TopScore = Score;
			}
			// ���� ������ ���� �ְ� ������ ���� ���
			else if (Score == TopScore)
			{
				StudentIDs.push_back(Id);
			}
		}

		cout << "\'" << Subject << "\' �ְ� ����: " << TopScore << "��" << endl;

		for (int Id : StudentIDs)
		{
			cout << "- �л� ��ȣ: " << Id << endl;
		}

		cout << endl;
	}

	// Ư�� ���񿡼� ������ ���� ����(�̻�, ����)�� �ش��ϴ� �л� ����� ���
	void PrintStudentsInScoreRange(string Subject, int MinScore, int MaxScore)
	{
		cout << endl;

		// �ش� ������ �����ϴ� �л��� ��ȣ
		vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);

		// �ش� ������ �����ϴ� �л��̰� ������ ���� ������ �ش��ϴ� �л� ���
		vector<int> StudentIDs;

		// �ش� ������ �����ϴ� �л��� ��ȣ ��ȸ
		for (auto& [Id, Score] : StudentScores)
		{
			// ������ ���� ������ �ش��ϴ� �л��� ���
			if (Score >= MinScore && Score <= MaxScore)
			{
				StudentIDs.push_back(Id);
			}
		}

		cout << "\'" << Subject << "\' ������ "<< MinScore << "�� ~ " << MaxScore<< "�� ������ �л� ��� : " << endl;

		for (int Id : StudentIDs)
		{
			cout << "- �л� ��ȣ: " << Id << endl;
		}

		cout << endl;
	}

	// Ư�� ������ �ְ�/���� ��� ���� �� ���� �ο��� ���
	void PrintSubjectScoreStats(string Subject)
	{
		cout << endl;

		// �ش� ������ �����ϴ� �л��� ��ȣ
		vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);

		// ���� �������� �������� ����
		sort(StudentScores.begin(), StudentScores.end(),
			[](pair<int, int> A, pair<int, int> B)
			{
				return A.second < B.second;
			});

		// ���� / �ְ� / ��� ����
		int MinScore = StudentScores.front().second, MaxScore = StudentScores.back().second;
		double AvgScore = 0.0;

		// �ش� ������ �����ϴ� �л��� ��ȣ ��ȸ
		for (auto& [Id, Score] : StudentScores)
		{
			AvgScore += Score;
		}

		AvgScore /= StudentScores.size();

		cout << "\'" << Subject << "\' ���� ����: " << MinScore << "��, �ְ� ����: " << MaxScore << "��, ��� ����: "<< AvgScore << endl;
		cout << "���� �л� ��ȣ ���: " << endl;

		// �ش� ������ �����ϴ� �л��� ��ȣ ��ȸ
		for (auto& [Id, Score] : StudentScores)
		{
			cout << "- �л� ��ȣ: " << Id << ", ����: " << Score << "��" << endl;
		}

		cout << endl;
	}
};


int main()
{
	// �л� ���� ���� ���α׷� ����
	StudentScoreManager Manager;

	while (true)
	{
		cout << "=======================" << endl;
		cout << "�л� ���� ���� ���α׷�" << endl;
		cout << "=======================" << endl;
		cout << "1. �л� ���� �߰�" << endl;
		cout << "2. �л� ���� ��ȸ" << endl;
		cout << "3. ��ü ��� ���� ���" << endl;
		cout << "4. ���� �ְ� ���� �л� ��ȸ" << endl;
		cout << "5. ���� ���� �˻�" << endl;
		cout << "6. ���� ���� ���" << endl;
		cout << "7. ����" << endl;
		cout << "���� : ";

		int Choice;
		cin >> Choice;

		if (Choice == 1)
		{
			// ��ȣ
			int StudentId;
			cout << "\n������ �߰��� �л��� ��ȣ�� �Է��ϼ���.(���� ����): ";
			cin >> StudentId;

			if (!cin || StudentId < 1)
			{
				cout << "\n�л� ��ȣ�� ���� ������ �Է� �����մϴ�." << endl;
				cout << endl;
				cin.clear();
				cin.ignore();
				continue;
			}

			// ����
			string Subject;
			cout << "\n������ �߰��� ������ �̸��� �Է��ϼ���.: ";
			// ���� �Է��� �ܿ� ���۸� ����
			cin.ignore();
			getline(cin, Subject);

			// ����
			int Score;
			cout << "\n������ �߰��� ������ ������ �Է��ϼ���.(0 ~ 100): ";
			cin >> Score;

			if (!cin || Score < 0 || Score > 100)
			{
				cout << "\n������ 0 ~ 100 ���� ������ �Է� �����մϴ�." << endl;
				cout << endl;
				cin.clear();
				cin.ignore();
				continue;
			}

			// ���� �߰�
			Manager.AddStudentScore(StudentId, Subject, Score);
		}
		else if (Choice == 2)
		{
			// ��ȣ
			int StudentId;
			cout << "\n������ ��ȸ�� �л��� ��ȣ�� �Է��ϼ���.(���� ����): ";
			cin >> StudentId;

			// �л� ���� ���
			Manager.PrintAllScoreByStudentId(StudentId);
		}
		else if (Choice == 3)
		{
			// ��ü ���� ��� ���� ���
			Manager.PrintAllSubjectAverageScore();
		}
		else if (Choice == 4)
		{
			// ����
			string Subject;
			cout << "\n�ְ� ���� �л����� ã�� ������ �̸��� �Է��ϼ���.: ";
			// ���� �Է��� �ܿ� ���۸� ����
			cin.ignore();
			getline(cin, Subject);

			if (Manager.IsTakingSubject(Subject))
			{
				// Ư�� ���� �ְ� ���� �л��� ���
				Manager.PrintTopStudentsBySubejct(Subject);
			}
		}
		else if (Choice == 5)
		{
			// ����
			string Subject;
			cout << "\n������ ��ȸ�� ������ �̸��� �Է��ϼ���.: ";
			// ���� �Է��� �ܿ� ���۸� ����
			cin.ignore();
			getline(cin, Subject);

			if (Manager.IsTakingSubject(Subject))
			{
				int MinScore, MaxScore;
				cout << "\n������ ��ȸ�� ���� ������ �Է��ϼ���.(0 ~ 100): ";
				cin >> MinScore;

				if (!cin || MinScore < 0 || MinScore > 100)
				{
					cout << "\n���� ������ 0 ~ 100 ���� ������ �Է� �����մϴ�." << endl;
					cout << endl;
					cin.clear();
					cin.ignore();
					continue;
				}

				cout << "\n������ ��ȸ�� �ְ� ������ �Է��ϼ���. (" << MinScore << " ~ 100): ";
				cin >> MaxScore;

				if (!cin || MaxScore < MinScore || MaxScore > 100)
				{
					cout << "\n�ְ� ������ " << MinScore << " ~ 100 ���� ������ �Է� �����մϴ�." << endl;
					cout << endl;
					cin.clear();
					cin.ignore();
					continue;
				}

				// �ش� ������ ����/�ְ� ������ ������ ���� �л��� ���
				Manager.PrintStudentsInScoreRange(Subject, MinScore, MaxScore);
			}
		}
		else if (Choice == 6)
		{
			// ����
			string Subject;
			cout << "\n������ ��踦 Ȯ���� ������ �̸��� �Է��ϼ���.: ";
			// ���� �Է��� �ܿ� ���۸� ����
			cin.ignore();
			getline(cin, Subject);

			if (Manager.IsTakingSubject(Subject))
			{
				// �ش� ������ ���(����/�ְ�/��� ����, ���� �л� ��ȣ)�� ���
				Manager.PrintSubjectScoreStats(Subject);
			}
		}
		else if (Choice == 7)
		{
			cout << "\n���α׷��� �����մϴ�." << endl;
			break; // while ���� ����
		}
		else
		{
			// �߸��� �Է� ó��
			// �޴��� ���� ��ȣ�� �Է����� ��� ��� �޽����� ����մϴ�.
			cout << "\n�߸��� �Է��Դϴ�. �ٽ� �õ��ϼ���.\n\n";
			cin.clear();
			cin.ignore();
		}
	}

	return 0;
}