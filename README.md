# 2강 숙제: 온라인 학습 관리 시스템 구현

## 구현할 기능

### **학생 성적 추가 기능**
  - `학생 ID(int)`와 `과목 이름(string)`, `점수(int)`를 저장한다.
  - 한 학생은 여러 과목을 수강할 수 있다.
      - (예: "1001번 학생이 'C++' 과목에서 85점, '알고리즘'에서 90점을 받음")
  - 동일 학생의 동일 과목을 입력하는 경우엔 최신 점수로 갱신을 한다.
  - 점수는 0 ~ 100점까지만 유효한 범위
    
### **학생의 전체 성적 조회 기능**
  - 특정 학생의 모든 과목 성적을 출력한다.
      - (예: "1001번 학생: C++(85점), 알고리즘(90점)")
  - 과목명은 알파벳 순으로 정렬하여 출력한다.
  - 또한, 존재하지 않는 학생 ID 입력 시 예외 처리를 하도록 한다.

### **전체 학생의 평균 점수 출력 기능**

  - 전체 학생들의 각 과목별 평균 점수를 출력한다.
  - 평균 점수는 소수점 둘째 자리까지 표시한다.
      - (예: "C++ 과목 평균 점수: 87.5, 알고리즘 과목 평균 점수: 92.3")

### **과목별 최고 점수 학생 조회 기능**

  - 특정 과목에서 가장 높은 점수를 받은 학생들을 찾는다.
  - 동점자가 있을 경우 학생 ID를 오름차순으로 정렬해서 모두 출력한다.
      - (예: "알고리즘 최고 점수: 95점 (학생 ID: 1003, 1005)")
   
### **추가 기능 : 성적 구간 검색 기능**
  - 특정 과목에서 지정된 점수 구간(이상, 이하)에 해당하는 학생 목록을 출력한다.

### **추가 기능 : 과목별 성적 통계 기능**
  - 특정 과목의 최고/최저 평균 점수 및 수강 인원을 출력한다.

---
---

## 구현 내용 설명

### 사용한 컨테이너 종류와 이유

- **map**과 **set**
  - 학생 번호 혹은 과목 등으로 `특정한 데이터(Key)`로 `필요한 데이터(Value)`를 가져오기 위해 사용했다.
    > 학생 번호(`key`)를 입력하여 해당 학생의 성적(`value`)를 가져오는 등에 사용했다.

  ```cpp

  // 학생 정보를 담을 맵
  // key = 학생 번호, value = 해당 학생의 성적(key = 과목, value = 해당 과목의 점수)
  map<int, map<string, int>> Students;

  // 학생들이 수강한 과목 목록
  set<string> Subjects;
  
  ```

- **vector**
  - 특정한 상황에 해당하는 학생 목록을 출력 할때 임시로 배열을 만들때 사용했다.
    > 특정 과목에서 가장 높은 점수를 받은 학생들 출력 할 경우, 특정 과목에서 지정된 점수 구간(이상, 이하)에 해당하는 학생 목록을 출력 할 경우 등에 사용했다.
  - `map`에 담겨있는 데이터를 정렬해서 사용하기 위해 사용했다.
    > 학생 번호를 기준으로 데이터 정렬, 특정 과목에 대해 학생들의 점수를 기준으로 데이터 정렬 등에 사용했다.

  ```cpp

  // 특정 과목을 수강한 학생 목록
  // first = 학생 번호, second = 해당 과목 점수
  // 학생 번호 기준 오름차순 정렬
  vector<pair<int, int>> GetStudentsBySubject(string Subject)
  {
  	// 해당 과목을 수강한 학생들의 번호와 점수
  	vector<pair<int, int>> StudentScores;
  
  	// 전체 학생 순회
  	for (auto& [Id, Scores] : Students)
  	{
  		// 해당 과목을 수강하는 학생일 경우
  		if (Scores.find(Subject) != Scores.end())
  		{
  			StudentScores.push_back({ Id, Scores[Subject] });
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

  ```
---

### 효율적인 데이터 관리를 위한 설계 방식

- 자주 사용하는 `조건`이나 `데이터` 등을 함수로 만들어서 필요할때 사용 할수 있도록 했다.

- **입력한 과목이 수강한 학생이 있는 과목인지 판별하는 함수**
  - 과목을 입력 받을 때 수강한 학생이 없는 과목이면 `false`를 반환 하는 함수

  ``` cpp

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
  
  ```

- **특정 과목을 수강한 학생 목록**
  - 해당 과목을 수강한 학생의 학생 번호와 해당 과목의 점수를 담은 벡터를 반환하는 함수

 ``` cpp

  // 특정 과목을 수강한 학생 목록
  // first = 학생 번호, second = 해당 과목 점수
  // 학생 번호 기준 오름차순 정렬
  vector<pair<int, int>> GetStudentsBySubject(string Subject)
  {
  	// 해당 과목을 수강한 학생들의 번호와 점수
  	vector<pair<int, int>> StudentScores;
  
  	// 전체 학생 순회
  	for (auto& [Id, Scores] : Students)
  	{
  		// 해당 과목을 수강하는 학생일 경우
  		if (Scores.find(Subject) != Scores.end())
  		{
  			StudentScores.push_back({ Id, Scores[Subject] });
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
  
  ```

- **특정 과목에 대한 평균 점수**
  - 해당 과목을 수강한 학생들의 전체 평균 점수를 반환하는 함수

  ```cpp

  // 특정 과목에 대한 평균 점수
  double GetAverageScoreBySubject(string Subject)
  {
  	vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);
  
  	// 해당 과목의 점수 합계
  	int SumScore = accumulate(StudentScores.begin(), StudentScores.end(), 0,
  		[](int A, pair<int, int> B)
  		{
  			return A + B.second;
  		});
  
  	// 해당 과목의 전체 평균
  	return (double)SumScore / (double)StudentScores.size();
  }
  
  ```

---  

### 사용한 STL알고리즘과 필요했던 이유

- **sort**
  - 데이터를 출력 할 떄 정렬된 데이터 목록으로 출력하기 위해 사용했다.

  ```cpp
  
  // 학생 번호 기준 오름차순으로 정렬
  sort(StudentScores.begin(), StudentScores.end(),
  	[](pair<int, int> A, pair<int, int> B)
  	{
  		return A.first < B.first;
  	});
  
  return StudentScores;
  
  ```

- **accumulate**
  - 평균 값을 구하기 위한 총합을 구할때 사용했다.

  ```cpp
  
  // 특정 과목에 대한 평균 점수
  double GetAverageScoreBySubject(string Subject)
  {
  	vector<pair<int, int>> StudentScores = GetStudentsBySubject(Subject);
  
  	// 해당 과목의 점수 합계
  	int SumScore = accumulate(StudentScores.begin(), StudentScores.end(), 0,
  		[](int A, pair<int, int> B)
  		{
  			return A + B.second;
  		});
  
  	// 해당 과목의 전체 평균
  	return (double)SumScore / (double)StudentScores.size();
  }
  
  ```












