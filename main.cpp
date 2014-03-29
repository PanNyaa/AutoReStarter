#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>




int chkfile(char*);
int strcnt(char*,const char*);
int strrep(char*,const char*,const char*);



int First(void);
int Check(int);
int ini(void);


char file_path[256];

int WINAPI WinMain(HINSTANCE hInstance ,HINSTANCE hPrevInstance ,PSTR lpCmdLine ,int nCmdShow){

	if( First() == -1 ){
		return -1;
	}

	while(1){

		if( Check(10) == 1){
			char cmdbuf[512];
			/*
			sprintf(cmdbuf,"スリープ復帰したので、\n%s\nを起動し直します。",file_path);
			MessageBox(NULL , cmdbuf , "AutoReStarter" , MB_OK | MB_TOPMOST);*/
			sprintf(cmdbuf,"\"%s\" ",file_path);
			system(cmdbuf);
			Check(10);	//チェック状態リセット
		}
		
		_sleep(1);	//これが無いと(CPU使用率が)大変なことになります

	}

	return 0;
}

int First(void){
	if( ini() == -1 ){
		return -1;
	}else{
		return 0;
	}
}



int Check(int stoptime){
	static time_t now_time=0;
	static int first_flag=0;
	int stop_flag=0;

	if(first_flag == 0){
		now_time = time(NULL);
		first_flag++;
	}

	if( ( time(NULL) - now_time ) >= stoptime ){
		stop_flag = 1;
	}else{
		stop_flag = 0;
	}

	now_time = time(NULL);
	return stop_flag;
}

int ini(void){

	FILE* fp;

	if ( (fp = fopen("AutoReStarter.ini", "r")) == NULL) {
		int flag;
		MessageBox(NULL , "iniファイルが存在しないか壊れています。\nAutoReStarter.ini というファイル名で、AutoReStarter.exeと同じディレクトリに置いてください。" , "Error" , MB_OK | MB_ICONERROR);
		flag = MessageBox(NULL , "新たに、AutoReStarter.iniを作成しますか？" , "Error" , MB_OKCANCEL | MB_ICONINFORMATION);
		if(flag == 1){
			if ( (fp = fopen("AutoReStarter.ini", "w")) == NULL) {
				MessageBox(NULL , "iniファイルを作成できませんでした。\n書き込み権限があるかどうか、ディスクが読み込み専用になっていないかどうか確認してください。" , "Error" , MB_OK | MB_ICONERROR);
				MessageBox(NULL , "終了します……(´・ω・`)" , "Error" , MB_OK | MB_ICONERROR);
				return -1;
			}else{
				MessageBox(NULL , "iniファイルの作成に成功しました。\nスリープ復帰後に実行し直したいアプリケーションのファイルパス(相対パス可)をiniファイルに書き込んでください。\nその後、もう一度AutoReStarter.exeを起動してください。" , "Error" , MB_OK | MB_ICONINFORMATION);
				MessageBox(NULL , "終了します。終了後、iniファイルが開きます(たぶん)" , "Error" , MB_OK | MB_ICONINFORMATION);
				fclose(fp);
				system("start notepad AutoReStarter.ini");
				return -1;
			}
		}else{
			MessageBox(NULL , "では作成せずに終了します。手動でやってねっ(しろめ)" , "Error" , MB_OK | MB_ICONERROR);
			return -1;
		}
	}else{
		char buf[256];
		
		if( fgets(buf,255,fp) == NULL ){
			MessageBox(NULL , "iniファイルに何も書かれていません。" , "Error" , MB_OK | MB_ICONERROR);
			MessageBox(NULL , "終了します。終了後iniファイルが開くので、スリープ復帰後に実行し直したいアプリケーションのファイルパス(相対パス可)を書き込んでください。" , "Error" , MB_OK | MB_ICONINFORMATION);
			fclose(fp);
			system("start notepad AutoReStarter.ini");
			return -1;
		}

		//ここまできてやっとiniからファイルパスの読み込み。

		strrep(buf,"\n","");	//改行を削除

		if( chkfile(buf) == 0 ){	//ファイルの存在チェックして存在していなかった場合
			MessageBox(NULL , "指定されたファイルパスが無効です。" , "Error" , MB_OK | MB_ICONERROR);
			MessageBox(NULL , "終了します。終了後iniファイルが開くので、スリープ復帰後に実行し直したいアプリケーションのファイルパス(相対パス可)を書き込んでください。" , "Error" , MB_OK | MB_ICONINFORMATION);
			fclose(fp);
			system("start notepad AutoReStarter.ini");
			return -1;
		}else{
			strcpy(file_path,buf);
		}
	}



	fclose(fp);
	return 0;
}



//ここからライブラリ的な関数

//ファイルの存在をチェックする関数。
//ファイルパスを渡して、存在していなかった場合 0 を返す。
//存在していた場合 1 を返す。
int chkfile(char* path){
	FILE* fp;
	if( (fp = fopen(path,"r")) == NULL ){
		return 0;
	}else{
		fclose(fp);
		return 1;
	}
}

//文字配列内に指定した文字がいくつ存在するかを返す関数
int strcnt(char *str,const char *c){

	char *p;
	int  len,i=0;
	
	len = strlen(c);

	for(;;){
		if( (p = strstr(str,c)) != NULL ){		//検索先文字列から指定文字がHITしたら
			i++;
			str = p+len+1;
		}else{
			break;
		}
	}
	return i;
}




//文字列から特定文字列を検索し指定の特定文字列に置き換える、ような関数。
//文字列が入った配列の大きさを指定して、置換結果がそれ以上になったらエラーを返します。
//置換後の文字数が同じもしくは少ない場合のみの対応です。
//大きくしてしまうと2次元以上にしていたときなどに、他の配列に食い込んでしまうので、
//元のサイズよりも大きくなったときはエラーを返します。(-1　か　必要なサイズ？)
int strrep(char *str,const char *search,const char *replace){

	char *p;
	int len,len_s,len_r,re_cnt;

	len = strlen(str);			//検索先文字列の長さを得る
	len_s = strlen(search);			//指定された検索する文字列の長さを得る
	len_r = strlen(replace);		//指定された置換する文字列の長さを得る
	re_cnt = strcnt(str,search);		//指定された検索する文字列の数を得る

	if(len_s < len_r){
		return -1;	//検索文字列よりも置換文字列のほうが多かった場合、結果配列が大きくなってしまうのでエラーを返す
	}

	for(;;){
		if( (p = strstr(str,search)) != NULL ){	//検索先文字列から指定文字がHITしたら
		
			strncpy(p,replace,len_r);	//置換文字数分だけ置換
			strcpy( p+len_r,p+len_s);	//置換しきらなかった部分を詰める
			
		}
		if(--re_cnt <= 0)
			break;				//検索文字列の回数分だけ、繰り返す
	}
	return 0;
}
