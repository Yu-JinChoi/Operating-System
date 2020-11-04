# Operating-System

### 과제 0
* * *
1. 클라우드에서 리눅스 가상머신을 생성한다.

    * J-Cloud 이용: http://jcloud.chonbuk.ac.kr/
    
2. 나만의 리눅스 커널을 빌드하여, 운영체제에 설치한다.

    * Build: compile and install  
    
### 과제 1
* * *
#### 커널 레벨 클립보드 서비스 구현: K-board

* 커널 내에 ring buffer 기반으로 클립을 관리하는 K-board 구현
    
* K-board에 접근하기 위한 시스템 콜 구현: enqueue, dequeue
    
* K-board를 사용하는 예제 프로그램 구현: copy, paste

### 과제 2
* * *
#### Proc FS를 이용한 K-Board v2 구현

* 커널 모듈을 기반으로 ProcFS 구현
        
        1. ProcFS의 등록, 해제 및 간단한 기능들의 예제 포함
        
        ○ 모듈이 등록될 때
          • /proc/foo-dir 디렉토리 및 /proc/foo-dir/foo 파일 생성
          • 두 개의 샘플 foo_info 구조체를 커널 메모리를 할당받아 작성하고, list로 유지
                
        2. 모듈이 해제될 때는 위의 두 파일과 관련 내용을 삭제함
        3. foo 파일을 읽으면, list 를 순회하며 foo_info 구조체의 내용을 출력함
    
* Proc FS를 기반으로 K-Board v2 구현

       1. os_kboard.c 에서 구현한 내용을 모듈로 이전
          • 예제 프로그램을 참고하여, 필요한 부분 수정, 확장하여 진행
          
       2. Enqueue, dequeue 인터페이스를 Proc FS의 write(), read()로 변경
          • /proc/kboard/writer
          
       3. Queue 내용에 대해 Read 만 수행할 수 있는 인터페이스 제공
          • /proc/kboard/reader
          
       4. Count 값과 버퍼 상태를 볼 수 있는 인터페이스도 구현
          • /proc/kboard/count, /proc/kboard/dump
    
       • 참고
         K-Board의 초기화는 모듈을 내리고 새로 로드할 때 수행 가능함

    
* K-Board v2를 이용한 동기화 기법의 구현 및 비교
