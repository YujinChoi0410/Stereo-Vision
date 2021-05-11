int Compute_Score(int x, int y, BYTE **Rimg, BYTE **Limg, int w, int h, int Win, int dx)
{
	int m, n;  //Win x Win 윈도우 내 픽셀index(m:x축index, n:y축index)
	int sum = 0; //좌우영상 윈도우 내 픽셀 간 차의 합을 저장할 변수
	int half = (int)(Win / 2);  //윈도우 중심픽셀로 부터 테두리픽셀까지 거리(=계산하지 않을 영상테두리 두께)

	//window 내 좌우 영상 픽셀값 차의 절댓값을 모두 더하기
	for (n = 0; n < Win; n++) {
		for (m = 0; m < Win; m++) {
			sum += abs(Rimg[y - half + n][x - half + m] - Limg[y - half + n][x + dx - half + m]);
		}
	}

	return sum;
}

BYTE Compute_Disparity(int x, int y, BYTE **Rimg, BYTE **Limg, int w, int h, int Win, int Dist)
{
	int dx, Score, min; //dx:우측영상픽셀로부터 좌측영상픽셀까지 떨어진 거리.
	BYTE disparity; //계산된 disparity 값을 넣을 변수

	//0~Dist만큼 떨어진 픽셀들의 score값 중 가장 작은 값의 index찾기. diparity = index. 
	min = 9999;
	for (dx = 0; dx < Dist; dx++) {
		Score = Compute_Score(x, y, Rimg, Limg, w, h, Win, dx);
		if (Score < min) {
			min = Score;
			disparity = dx;
		}
	}

	return disparity;
}


void CHomework::OnClickedStereo()
{
	// 각종 변수 선언하기
	int x0, y0, w, h, x, y;
	//Result:normalize후 disparity map, Result1: 7*7 normalize전 disparity map, Result2: 21*21 normalize전 disparity map
	BYTE **Limg, **Rimg, **Result, **Result1, **Result2;

	Display_BMP_File("c:\\Rimg2.bmp", 0, 0); //Rimg1 BITMAP파일을 읽어서 (0,0)위치에 디스플레이
	GetCurrentImageInfo(&w, &h, &x0, &y0); // 입력영상 정보 읽기
	Rimg = cmatrix(h, w); // BYTE** 자료형 할당하기
	GetCurrentImageGray(Rimg); // 회색조 영상 읽기

	Display_BMP_File("c:\\Limg2.bmp", w, 0); //Limg1 BITMAP파일을 읽어서 (w,0)위치에 디스플레이	
	GetCurrentImageInfo(&w, &h, &x0, &y0); // 입력영상 정보 읽기
	Limg = cmatrix(h, w); // BYTE** 자료형 할당하기
	GetCurrentImageGray(Limg); // 회색조 영상 읽기

	//BYTE** 자료형 할당하기
	Result1 = cmatrix(h, w);
	Result2 = cmatrix(h, w);
	Result = cmatrix(h, w);

	//처음에 Disparity map의 모든 픽셀을 0으로 초기화 --> 계산하지 않을 픽셀을 0으로 만들어주기 위함.
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			Result1[y][x] = 0;
			Result2[y][x] = 0;
		}
	}

	//7*7 window로 Disparity 계산한 값을 Result1에 대입하기.
	for (y = 3; y < h - 3; y++) {
		for (x = 3; x < w - 3 - 30; x++) {
			Result1[y][x] = Compute_Disparity(x, y, Rimg, Limg, w, h, 7, 30);
		}
	}

	//21*21 window로 Disparity 계산한 값을 Result2에 대입하기.
	for (y = 10; y < h - 10; y++) {
		for (x = 10; x < w - 10 - 30; x++) {
			Result2[y][x] = Compute_Disparity(x, y, Rimg, Limg, w, h, 21, 30);
		}
	}

	//7*7 결과를 화면에 출력하기 위해 normalize하기. BYTE to BYTE.
	CNormalize2D(Result1, Result, w, h);

	//7*7 Disparity map을 디스플레이
	DisplayCimage2D(Result, w, h, x0 - w, y0 + h, true);

	//21*21 결과를 화면에 출력하기 위해 normalize하기. BYTE to BYTE.
	CNormalize2D(Result2, Result, w, h);

	//21*21 Disparity map을 디스플레이
	DisplayCimage2D(Result, w, h, x0, y0 + h, true);

	//BYTE** 자료형 할당 해제하기
	free_cmatrix(Limg, h, w);
	free_cmatrix(Rimg, h, w);
	free_cmatrix(Result1, h, w);
	free_cmatrix(Result2, h, w);
	free_cmatrix(Result, h, w);
}