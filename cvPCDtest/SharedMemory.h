#ifndef _INC_SMEM
#define _INC_SMEM

#include <windows.h>
#include <string>

using namespace std;

/******************************************
 *-----共有メモリを使用する為のクラス-----*
 ******************************************/
template <typename T > 
class SharedMemory{
private:
	HANDLE hShMem;	//ファイルマッピングのハンドル保存用
	const std::string FILEMAPNAME;	//共有するファイル名(共有するプロセスで統一する)
	T *datap;	//共有メモリへのポインタ
	bool mIsCreated;	//オブジェクトを新規作成したかどうか

	//共有メモリへのポインタを取得する
	void getShMem();
	//ファイルマップビューとハンドルを破棄する
	void releaseShMem();

public:
	//コンストラクタ
	//引数にファイル名が必要
	SharedMemory(const std::string FMNAME);
	//デストラクタ
	~SharedMemory();

	//共有メモリにデータを書き込む
	void setShMemData( T setData , int offset = 0 );
	//データを取得
	T getShMemData( int offset = 0 );

	////オブジェクトを新規作成したかどうか取得
	bool isCreated();

};

//↓↓↓↓ 以下実装部分 ↓↓↓↓
template < typename T >
SharedMemory<T>::SharedMemory(const std::string FMNAME) :FILEMAPNAME(FMNAME)
{
	SharedMemory::getShMem();
}

template < typename T >
SharedMemory<T>::~SharedMemory()
{
	SharedMemory::releaseShMem();
}

template < typename T >
void SharedMemory<T>::getShMem()
{
	/*
	* CreateFileMappingは指定されたファイルに対する、
	* 名前付きまたは名前なしのファイルマッピングオブジェクトを作成または開きます。
	*
	* HANDLE CreateFileMapping(
	* HANDLE hFile,                       // ファイルのハンドル
	* LPSECURITY_ATTRIBUTES lpAttributes, // セキュリティ
	* DWORD flProtect,                    // 保護
	* DWORD dwMaximumSizeHigh,            // サイズを表す上位 DWORD
	* DWORD dwMaximumSizeLow,             // サイズを表す下位 DWORD
	* LPCTSTR lpName                      // オブジェクト名
	* );
	*
	* ref=>https://msdn.microsoft.com/ja-jp/library/Cc430039.aspx
	*/
	hShMem = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		1024,
		FILEMAPNAME.c_str());

	if (hShMem != NULL)
	{
		//既にオブジェクトが作成されていたかどうか取得
		mIsCreated = GetLastError() == ERROR_ALREADY_EXISTS;

		/*
		* MapViewOfFileは呼び出し側プロセスのアドレス空間に、ファイルのビューをマップします。
		*
		* LPVOID MapViewOfFile(
		* HANDLE hFileMappingObject,   // ファイルマッピングオブジェクトのハンドル
		* DWORD dwDesiredAccess,       // アクセスモード
		* DWORD dwFileOffsetHigh,      // オフセットの上位 DWORD
		* DWORD dwFileOffsetLow,       // オフセットの下位 DWORD
		* SIZE_T dwNumberOfBytesToMap  // マップ対象のバイト数
		* );
		*
		* ref=>https://msdn.microsoft.com/ja-jp/library/Cc430198.aspx
		*/
		datap = (T *)MapViewOfFile(
			hShMem,
			FILE_MAP_WRITE,
			0,
			0,
			0);
	}
}

template < typename T >
void SharedMemory<T>::releaseShMem()
{
	if (hShMem != NULL)
	{
		if (datap != NULL)
			UnmapViewOfFile(datap);
		CloseHandle(hShMem);
		hShMem = NULL;
	}
}

template < typename T >
void SharedMemory<T>::setShMemData(T setData, int offset)
{
	*(datap + offset) = setData;
}

template < typename T >
T SharedMemory<T>::getShMemData(int offset)
{
	return *(datap + offset);
}

template < typename T >
bool SharedMemory<T>::isCreated()
{
	return mIsCreated;
}


#endif