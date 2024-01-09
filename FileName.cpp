#include <iostream>
#include <ctime>
#include <iomanip>


using namespace std;


// 시작할 땐 필요한 메소드와 변수들을 전역변수로 선언합니다.
void interdraw_One(int, int); // 맵의크기를 받고 예시로서 1번만 사용할 용도의 메소드
void interdraw(int**, int, int); //지속적으로 바뀔 맵의 원소들을 표시하기 위한 메소드
int countMines(int**, int, int, int, int); // 주변 지뢰를 찾는 메소드

int xCnt, yCnt; //사용자가 입력한 X,Y의 크기값

int** map; //맵을 설정하기 위해 필요한 map이라는 포인터 변수입니다. , 실제로 보여주고 사용할 배열
int** mine; //지뢰를 설정하기 위해 필요한 mine  포인터 변수입니다. , 마인이 실제 설치된 위치를 의미하는 배열
// map은 어디까지나 map으로써 지뢰의 위치를 모르지만 ,mine에는 모든 지뢰가 랜덤하게 보이는상태이다. 그러므로
// 배열을 2개 설정한 후, 지뢰를 통한 상호작용은 mine, 겉으로는 map을 보여주는 형태로 코드가 돌아간다.

int countMines(int** mine, int xCnt, int yCnt, int x, int y) { //마인을 세는 메소드
    int count = 0;

    // 여기서 말하는 count는 주변의 지뢰갯수를 의미하는데,
    // 지뢰가 모두 한 배열에 위치되어 있다는 점을 이용했다.
    // 지정된 위치(사용자로부터 받은x,y값) 로부터 좌우, 상하의 좌우, 상하의 위치를 모두 count해서
    // 있는 만큼 숫자를 증진시키고 int로써 내뱉는 원리이다, (&& = and연산자), (|| = or 연산자)
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int newX = x + i;
            int newY = y + j;

            if (newX >= 0 && newX < xCnt && newY >= 0 && newY < yCnt) {
                if (mine[newX][newY] == 1) {
                    count ++;
                }
            }
        }
    }

    return count;
}
//한번만 출력하는 맵, 미리보기개념
void interdraw_One(int xLen, int yLen) {  //xLen과 yLen은 여기서만 사용합니다.
   
    // 최초 1번만 설정하고 사용하지 않는 메소드기에 특수한 변수명을 사용했다.
    // 용도 : 전체 맵 대강 미리보기

    cout << "해당 맵에서 지뢰를 찾습니다."<< "\n" << "(실제값은 0이 추가됩니다.)" << endl;
    for (int i = 0; i <= yLen; i++) {
        cout << setw(2) << i;
    }
    cout << endl;

    for (int i = 1; i <= yLen; i++) {
        cout << setw(2) << i;
        for (int j = 0; j <= xLen; j++) {
            cout << setw(2) << " ";
        }
        cout << endl;
    }
} 

int MineCheck() {
    // 지뢰가 0이거나 범위보다 클 경우엔 다시 입력 받는다.
    // 허용된 지뢰값만을 허용하기 위한 메소드이다.
    // 해당 메소드로부터 마인크기를 입력받기에 매개변수가 따로 없다.
    // 만약 0이하거나 처음에 지정한x,y값보다도 큰 경우에는 else로 이동하지 않고 무한반복되기에
    // 올바른 지뢰 갯수를 입력받을 수 있게 된다.
    int mineCnt;
    do {
        cout << "생성할 지뢰의 총 개수를 입력해주세요." << endl;
        cin >> mineCnt;
            if (mineCnt <1) {
                cout << "0이하의 수를 제외한 수를 입력해주세요." << endl;
            }
            else if (mineCnt > xCnt * yCnt) {
                cout << "범위보다 크게 지뢰를 설치할 수 없어요!" << endl;
            }
            else{
                return mineCnt;
        }
    } while (true);
}


    // 지뢰 생성
void MakeTrap(int mineCnt) {
    int a, b = 0;
    // 해당 메소드에서는 a,b는 각각 x와 y좌표를 의미하여 실제로 지뢰를
    // 맵에 설치하는 메소드가 된다. 
    // 설치할 위치는 rand를 돌려서 랜덤한 곳에 설치하게 되는데 그 범위는 전역변수로 설정된 x와 y이하이며
    // 설치할 갯수만을 매개변수로 들고온다.
    for (int i = 0; i < mineCnt; i++) {
        while (1) {
            a = rand() % xCnt;
            b = rand() % yCnt;
            if (map[a][b] == 0) {
                mine[a][b] = 1;
                break;
            }
        }
    }
}


    //실제 맵 구성
void interdraw(int** map, int xCnt, int yCnt) {
    // x 좌표 출력 (열 , < 좌~우 > )
    cout << " ";
    for (int i = 0; i < xCnt; i++) {
        cout << setw(4) << "" << i;
    }
    cout << endl;

    // y 좌표 출력  (행, 위 ~ 아래)
    for (int i = 0; i < yCnt; i++) {
        cout << setw(2) << i;


        // 해당 기호들은 이미 밟은 곳, 혹은 지뢰가 누설된 위치를 의미하게된다.
        // map의 x와 y좌표가 0일경우는 아직 미확인, 1인경우는 근처에 지뢰가 있다는 의미(깃발의 하위호환), 2인 경우는 확인이 완료된 안전한 지역이라는 의미.
      
        for (int j = 0; j < xCnt; j++) {
            if (map[i][j] == 0) {
                cout << setw(6) << " ■ ";
            } else if (map[i][j] == 1) {
                cout << setw(6) << " ※ ";
            } else if (map[i][j] == 2) {
                cout << setw(6) << " ● ";
            }
        }
        cout << endl;
    }
}



int main() {
    
    srand((unsigned)time(NULL)); 
    // 프로그램을 실행하는 즉시 시드값을 바꾸면서 랜덤값이 생성되는 형태. 
    // 실행 시 1번만 실행되므로 꽤 효율적인 것 같다.

    cout << "생성할 맵의 X,Y값을 입력해주세요. ex) 3 3" << endl;    
    cin >> xCnt >> yCnt;  //사용자로부터 x,y를 입력받고 해당 값은 처음에 전역변수로써 설정되었다.

    interdraw_One(xCnt, yCnt);  // 최초 1번만을 출력하는 메소드, 미리보기개념

    // 배열 모든 값 초기화(사용자가 초기화하는 형태라고도 할 수 있다.)
    map = new int* [xCnt];
    mine = new int* [xCnt];  //사용자로부터 입력받은 x,y를 실제로 적용시키고 각각 맵의 크기, 지뢰의 최대값으로써 저장한다.

    for (int i = 0; i < yCnt; i++) { //  x만큼 y를 반복시켜서 2차원의 최대 맵의 크기, 최대 지뢰 설치 지역을 설정한다. 
        map[i] = new int[yCnt]();
        mine[i] = new int[yCnt]();
    }

   int mineCnt = MineCheck(); // mineCnt 변수는 MineCheck를 통해 Count된 갯수를 나타내는 변수,
                              // Count는 지금까지 몇 번 땅을 밟았는지 알려주는 변수이다.
    MakeTrap(mineCnt);
   int Count = 0; 

    while (1) {
        int x, y;
        int result = 0;

        cout << "x좌표와 y좌표를 입력하세요 : ex) 2 2 " << "현재까지 움직인 횟수 : " << Count << endl << "※는 주변에 지뢰가 있음을 의미합니다." << endl;
        cin >> y >> x;
        //  여기서 말하는 x좌표와 y좌표는 처음에 설정했던 "설정"과는 관계가 없다.
        //  자신이 밟을 위치이다.
        // 깃발을 꽂는 형태로 승리하는 형태도 있지만, 코드가 복잡하다고 생각해서
        // 지뢰를 "찾는다"는 요소를 재미로써 구현해보았다.

        
    
        if (x >= 0 && x < xCnt && y >= 0 && y < yCnt) { 
            // 만약 최초로 밟은 위치에 지뢰가 설치되어있었다면
            // 초기화하고 처음부터 다시 실행하는 부분이 된다.
            // 초기화는 크기부터 재지정 하는 것이 아니라 처음에 입력받은 값을 그대로 이용해서 지뢰의 위치만 바꾸는 형태이다.
            if (map[x][y] == 1 && (Count == 0)) {
                cout << "첫 땅이 지뢰네요. 지뢰를 다시 설치 중.." << endl;

                for (int i = 0; i < xCnt; i++) {
                    for (int j = 0; j < yCnt; j++) {
                        map[i][j] = 0; 
                        mine[i][j] = 0;
                    }
                }
                MakeTrap(mineCnt);
                Count = 0;
                cout << " \n";
             
                interdraw(map, xCnt, yCnt);
                cout << " \n";
                continue;
            }

            else if (mine[x][y] == 1 && (Count > 1)) {
               // 최초로 밟지 않았기에 그 다음부터 밟는 경우는 지뢰를 밟은 것으로 판단,
               // 사망처리 후 전체 지뢰 위치를 보여준 후에 게임을 종료시킨다.
                map[x][y] = 1; 

                cout << " 지뢰가 펑~하고 터졌습니다!!!!" << endl << "결과창" << endl;
                interdraw(mine, xCnt, yCnt);
                break;
            }
     
            else if (map[x][y] == 2) {
                //단순하게 이미 밟은 곳은 알려준다.
                cout << "이미 방문한 지역이에요!"  << endl;
            }
            else if (map[x][y] == 0) {
                // 최초로 밟은 경우 안전함을나타냄과  동시에
                // 주변 위치에 존재하는 지뢰의 갯수를 알려주게되며,
                // 해당 map의 좌표를 지뢰가 있을경우는 폭탄표시, 
                // 그렇지 않으면 주변이 안전하다는 동그라미표시를 위해 1(주의) 또는 2(안전)를 남긴다.
                int mineCount = countMines(mine, xCnt, yCnt, x, y);
                
                if (mineCount > 0) {
                    map[x][y] = 1;
                }else map[x][y] = 2;
                 cout << "주변 지뢰 개수: " << mineCount << endl;
                result++;
                Count++;
            }

            
            interdraw(map, xCnt, yCnt);

            if ((xCnt * yCnt - mineCnt) == result) {
                //승리조건은 지뢰를 모두 찾았으며 더 이상 안전하게 밟을 땅이 없다는 의미가 된다.
                cout << "지뢰를 모두 찾았다!" << endl;
                Count = 0;
                break;
            }
        }
        else {
            // 일반적으로는 0,0 0,1으로 들어오지만, 
            // 만약 맵 크기보다 크거나 작은 경우에는 x와 y를 자동 초기화하고 좌표를 다시 입력하게끔 만드는 부분이다.
            cout << "유효하지 않은 좌표입니다. 다시 입력하세요." << endl;
            x = -1;
            y = -1;
        }
    }


}
