#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>


static int first_flag=0;		//ごめんなさいごめんなさいCheck(int)で使っていたのをグローバル変数にしました


int chkfile(char*);
int strcnt(char*,const char*);
int strrep(char*,const char*,const char*);



int First();
int Check(int);
int ini(void);

char dir_path[256];
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
			/*
			sprintf(cmdbuf,"cd \"%s\" ",dir_path);
			system(cmdbuf);
			sprintf(cmdbuf,"cd \"%s\" ",dir_path);
			system(cmdbuf);*/

			sprintf(cmdbuf,"\"%s\" ",file_path);
			system(cmdbuf);
			first_flag=0;
		}else{
			_sleep(1);	//これが無いと(CPU使用率が)大変なことになります
		}

	}

	return 0;
}

int First(){
	if( ini() == -1 ){
		return -1;
	}else{
		return 0;
	}
}



int Check(int stoptime){
	static time_t now_time=0;

	static int stop_flag=0;

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
			for(int i=0;;i++){
				if( buf[255 - i] != '\\' && buf[255 - i] != '/' ){
					buf[255 - i] = '\0';
				}else{break;}
			}
			strcpy(dir_path,buf);
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
	/*
	int i=0,j=0;
	for(;*str != '\0' && size != j;*str++, j++){
		if(*str == c)
			i++;
	}
	return i;
	*/
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

	/*　実装しましょう　*/

	char *p,*p_strend;
	int len,len_s,len_r,re_cnt;

	len = strlen(str);						//検索先文字列の長さを得る
	len_s = strlen(search);			//指定された検索する文字列の長さを得る
	len_r = strlen(replace);			//指定された置換する文字列の長さを得る
	p_strend = str + len;				//文字列終端のポインタを得る
	re_cnt = strcnt(str,search);		//指定された検索する文字列の数を得る

	if(len_s < len_r){
		return -1;	//検索文字列よりも置換文字列のほうが多かった場合、結果配列が大きくなってしまうのでエラーを返す
	}

	for(;;){

		if( (p = strstr(str,search)) != NULL ){		//検索先文字列から指定文字がHITしたら
		
			strncpy(p,replace,len_r);	//置換文字数分だけ置換
		
#ifdef COMENNTOUT
		//p+=*search;	//strncpyをした時点でのポインタの位置は、置換した文字の先頭？にあるので、置換前文字数分、アドレス移動
		for(;;){
			/*　https://twitter.com/pan_nyaa/status/430581427516620800　アイディアをまとめた　*/
			/*　文字列終端のポインタを取得して、現在ポインタとfseekしてその区間分の可変長配列を生成してそっちに退避させて、元文字列と退避文字列の間の\0を削除して連結、っていうアルゴリズム　*/
			/*　いまのままだと、置換処理はできてるけど、strncpyの仕様上\0で埋まってしまうのでそこで文字列が途切れてしまう。　*/
			/*　そこで、↑にまとめた処理を挟んで連結させてみる　*/
			/*char *_str;
			if( ( _str = (char *)malloc(pseek(p+len_s,p_strend) ) ) == NULL)return -1;*/

			strcpy( p,p+len_s);
			//strcpy( str + pseek(str,p),p );
			break;
		}
#endif
			strcpy( p+len_r,p+len_s);	//これだけでいいですね(まっしろめ)
		/*　p+len_r　が指し示す位置は、置換後文字列部分の終端+1の場所。p+len_s　が指し示す位置は、置換前文字列の終端+1　*/
		/*　つまり、OOOOXXXOOO　のXXXをTTに変えるとき、*/
		/*　　　　　OOOOXXXOOO　が　*/
		/*　　　　　OOOOTTXOOO　になり、（まだ余剰分はそのまま）　*/
		/*　　　　　OOOOTTOOO　になる。（余剰分が詰められる）　*/
		}
		if(--re_cnt <= 0)
			break;			//検索文字列の回数分だけ、繰り返す
	}
	return 0;
}
