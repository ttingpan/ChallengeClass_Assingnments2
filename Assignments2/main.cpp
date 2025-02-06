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
	// 학생 정보를 담을 맵
	// key = 학생 번호, value = 해당 학생의 성적(key = 과목, value = 해당 과목의 점수)
	map<int, map<string, int>> Students;

	// 학생들이 수강한 과목 목록
	set<string> Subjects;

public:
	// 학생 점수 추가
	void AddStudentScore(int Id, string Subject, int Score)
	{
		cout << endl;

		// 과목 추가
		Subjects.insert(Subject);

		// 성적 최신화
		Students[Id][Subject] = Score;

		cout << Id << "번 학생의 \'" << Subject << "\'의 점수가 " << Score << "(으)로 최신화 되었습니다." << endl;

		cout << endl;
	}

	// 특정 학생의 모든 과목 성적 출력
	void PrintAllScoreByStudentId(int Id)
	{
		cout << endl;

		// 입력한 번호의 학생이 없을 경우
		if (Students.find(Id) == Students.end())
		{
			cout << Id << "번 학생의 정보가 없습니다." << endl;
			cout << endl;
			return;
		}

		// 특정 학생의 모든 과목 성적
		map<string, int> Scores{ Students[Id] };

		cout << Id << "번 학생의 성적: " << endl;

		for (auto& [Subject, Score] : Scores)
		{
			cout << "- " << Subject << ": " << Score << "점" << endl;
		}

		cout << endl;
	}

	// 각 과목별 전체 학생의 평균 점수 출력
	void PrintAllSubjectAverageScore()
	{
		cout << endl;

		// 과목별 점수 합계
		map<string, int> SumScores;
		// 과목별 학생 수
		map<string, int> StudentCount;

		// 전체 학생 순회
		for (auto& [Id, Scores] : Students)
		{
			// 각 학생별 성적
			for (auto& [Subject, Score] : Scores)
			{
				// 해당 과목별 점수 합계에 해당 학생의 점수 추가
				SumScores[Subject] += Score;
				// 해당 과목별 학생 수 증가
				StudentCount[Subject]++;
			}
		}

		cout << "전체 과목 평균 점수: " << endl;

		// 학생들이 수강한 모든 과목 순회
		for (string Subject : Subjects)
		{
			// 해당 과목의 전체 평균
			double Avg = (double)SumScores[Subject] / (double)StudentCount[Subject];

			// 소수점 둘째 자리까지 표현하기 위해 계산
			Avg *= 100;
			Avg = round(Avg);
			Avg /= 100;

			cout << "- " << Subject << ": " << Avg << "점" << endl;
		}

		cout << endl;
	}

	// 해당 과목이 저장된 과목인지 확인(해당 과목을 수강한 학생이 있는지 확인)
	bool IsTakingSubject(string Subject)
	{
		// 저장된 과목이 아닌 경우(해당 과목을 수강한 학생이 없는 경우)
		if (Subjects.find(Subject) == Subjects.end())
		{
			cout << "\n해당 과목을 수강한 학생이 없습니다." << endl;
			cout << endl;
			return false;
		}

		return true;
	}

	// 특정 과목을 수강하는 학생 목록
	// first = 학생 번호, second = 해당 과목 점수
	// 학생 번호 기준 오름차순 정렬
	vector<pair<int, int>> GetStudentsBySubject(string Subject)
	{
		// 해당 과목을 수강하는 학생들의 번호와 점수
		vector<pair<int, int>> StudentScores;

		// 전체 학생 순회
		for (auto& [Id, Scores] : Students)
		{
			// 해당 과목을 수강하는 학생일 경우
			if (Scores.find(Subject) != Scores.end())
			{
				StudentScores.push_back({Id, Scores[Subject]});
			}
		}

		// 학생 번호 기준 오름차순으로 정렬
		sort(StudentScores.begin(), StudentScores.end(),
			[](pair<int, int> A, pair<int, int> B)
			{
				return A.first < B.first;
			});

		return StudentScores;
	}

	// 특정 과목에서 가장 높은 점수를 받은 학생들 출력
	void PrintTopStudentsBySubejct(string Subject)
	{
		cout << endl;

		// 해당 과목을 수강하는 학생들의 번호와 점수
		vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);
		
		// 최고 점수 학생들의 번호
		vector<int> StudentIDs;

		// 최고 점수
		int TopScore = 0;

		// 전체 학생 순회
		for (auto& [Id, Score] : StudentScores)
		{
			// 현재 점수가 이전 최고 점수보다 클 경우
			if (Score > TopScore)
			{
				StudentIDs.clear();
				StudentIDs.push_back(Id);
				TopScore = Score;
			}
			// 현재 점수가 이전 최고 점수와 같을 경우
			else if (Score == TopScore)
			{
				StudentIDs.push_back(Id);
			}
		}

		cout << "\'" << Subject << "\' 최고 점수: " << TopScore << "점" << endl;

		for (int Id : StudentIDs)
		{
			cout << "- 학생 번호: " << Id << endl;
		}

		cout << endl;
	}

	// 특정 과목에서 지정된 점수 구간(이상, 이하)에 해당하는 학생 목록을 출력
	void PrintStudentsInScoreRange(string Subject, int MinScore, int MaxScore)
	{
		cout << endl;

		// 해당 과목을 수강하는 학생들의 번호와 점수
		vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);

		// 해당 과목을 수강하는 학생이고 지정된 점수 구간에 해당하는 학생 목록
		vector<int> StudentIDs;

		// 해당 과목을 수강하는 학생들 번호 순회
		for (auto& [Id, Score] : StudentScores)
		{
			// 지정된 점수 구간에 해당하는 학생일 경우
			if (Score >= MinScore && Score <= MaxScore)
			{
				StudentIDs.push_back(Id);
			}
		}

		cout << "\'" << Subject << "\' 점수가 "<< MinScore << "점 ~ " << MaxScore<< "점 사이인 학생 목록 : " << endl;

		for (int Id : StudentIDs)
		{
			cout << "- 학생 번호: " << Id << endl;
		}

		cout << endl;
	}

	// 특정 과목의 최고/최저 평균 점수 및 수강 인원을 출력
	void PrintSubjectScoreStats(string Subject)
	{
		cout << endl;

		// 해당 과목을 수강하는 학생들의 번호와 점수
		vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);

		// 점수 기준으로 오름차순 정렬
		sort(StudentScores.begin(), StudentScores.end(),
			[](pair<int, int> A, pair<int, int> B)
			{
				return A.second < B.second;
			});

		// 최저 / 최고 / 평균 점수
		int MinScore = StudentScores.front().second, MaxScore = StudentScores.back().second;
		

		// 해당 과목을 수강하는 학생들의 점수 총합
		int SumScore = accumulate(StudentScores.begin(), StudentScores.end(), 0);
		double AvgScore = (double) SumScore / (double) StudentScores.size();

		cout << "\'" << Subject << "\' 최저 점수: " << MinScore << "점, 최고 점수: " << MaxScore << "점, 평균 점수: "<< AvgScore << endl;
		cout << "수강 학생 번호 목록: " << endl;

		// 해당 과목을 수강하는 학생들 번호 순회
		for (auto& [Id, Score] : StudentScores)
		{
			cout << "- 학생 번호: " << Id << ", 점수: " << Score << "점" << endl;
		}

		cout << endl;
	}
};


int main()
{
	// 학생 성적 관리 프로그램 생성
	StudentScoreManager Manager;

	while (true)
	{
		cout << "=======================" << endl;
		cout << "학생 성적 관리 프로그램" << endl;
		cout << "=======================" << endl;
		cout << "1. 학생 성적 추가" << endl;
		cout << "2. 학생 성적 조회" << endl;
		cout << "3. 전체 평균 점수 출력" << endl;
		cout << "4. 과목별 최고 점수 학생 조회" << endl;
		cout << "5. 성적 구간 검색" << endl;
		cout << "6. 과목별 성적 통계" << endl;
		cout << "7. 종료" << endl;
		cout << "선택 : ";

		int Choice;
		cin >> Choice;

		if (Choice == 1)
		{
			// 번호
			int StudentId;
			cout << "\n성적을 추가할 학생의 번호를 입력하세요.(양의 정수): ";
			cin >> StudentId;

			if (!cin || StudentId < 1)
			{
				cout << "\n학생 번호는 양의 정수만 입력 가능합니다." << endl;
				cout << endl;
				cin.clear();
				cin.ignore();
				continue;
			}

			// 과목
			string Subject;
			cout << "\n성적을 추가할 과목의 이름을 입력하세요.: ";
			// 이전 입력의 잔여 버퍼를 제거
			cin.ignore();
			getline(cin, Subject);

			// 성적
			int Score;
			cout << "\n성적을 추가할 과목의 점수를 입력하세요.(0 ~ 100): ";
			cin >> Score;

			if (!cin || Score < 0 || Score > 100)
			{
				cout << "\n점수는 0 ~ 100 사이 점수만 입력 가능합니다." << endl;
				cout << endl;
				cin.clear();
				cin.ignore();
				continue;
			}

			// 성적 추가
			Manager.AddStudentScore(StudentId, Subject, Score);
		}
		else if (Choice == 2)
		{
			// 번호
			int StudentId;
			cout << "\n성적을 조회할 학생의 번호를 입력하세요.(양의 정수): ";
			cin >> StudentId;

			// 학생 성적 출력
			Manager.PrintAllScoreByStudentId(StudentId);
		}
		else if (Choice == 3)
		{
			// 전체 과목 평균 점수 출력
			Manager.PrintAllSubjectAverageScore();
		}
		else if (Choice == 4)
		{
			// 과목
			string Subject;
			cout << "\n최고 점수 학생들을 찾을 과목의 이름을 입력하세요.: ";
			// 이전 입력의 잔여 버퍼를 제거
			cin.ignore();
			getline(cin, Subject);

			if (Manager.IsTakingSubject(Subject))
			{
				// 특정 과목 최고 점수 학생들 출력
				Manager.PrintTopStudentsBySubejct(Subject);
			}
		}
		else if (Choice == 5)
		{
			// 과목
			string Subject;
			cout << "\n성적을 조회할 과목의 이름을 입력하세요.: ";
			// 이전 입력의 잔여 버퍼를 제거
			cin.ignore();
			getline(cin, Subject);

			if (Manager.IsTakingSubject(Subject))
			{
				int MinScore, MaxScore;
				cout << "\n성적을 조회할 최저 점수를 입력하세요.(0 ~ 100): ";
				cin >> MinScore;

				if (!cin || MinScore < 0 || MinScore > 100)
				{
					cout << "\n최저 점수는 0 ~ 100 사이 점수만 입력 가능합니다." << endl;
					cout << endl;
					cin.clear();
					cin.ignore();
					continue;
				}

				cout << "\n성적을 조회할 최고 점수를 입력하세요. (" << MinScore << " ~ 100): ";
				cin >> MaxScore;

				if (!cin || MaxScore < MinScore || MaxScore > 100)
				{
					cout << "\n최고 점수는 " << MinScore << " ~ 100 사이 점수만 입력 가능합니다." << endl;
					cout << endl;
					cin.clear();
					cin.ignore();
					continue;
				}

				// 해당 과목의 최저/최고 사이의 점수를 가진 학생들 출력
				Manager.PrintStudentsInScoreRange(Subject, MinScore, MaxScore);
			}
		}
		else if (Choice == 6)
		{
			// 과목
			string Subject;
			cout << "\n성적을 통계를 확인할 과목의 이름을 입력하세요.: ";
			// 이전 입력의 잔여 버퍼를 제거
			cin.ignore();
			getline(cin, Subject);

			if (Manager.IsTakingSubject(Subject))
			{
				// 해당 과목의 통계(최저/최고/평균 점수, 수강 학생 번호)를 출력
				Manager.PrintSubjectScoreStats(Subject);
			}
		}
		else if (Choice == 7)
		{
			cout << "\n프로그램을 종료합니다." << endl;
			break; // while 루프 종료
		}
		else
		{
			// 잘못된 입력 처리
			// 메뉴에 없는 번호를 입력했을 경우 경고 메시지를 출력합니다.
			cout << "\n잘못된 입력입니다. 다시 시도하세요.\n\n";
			cin.clear();
			cin.ignore();
		}
	}

	return 0;
}