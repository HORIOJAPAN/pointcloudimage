// Comm.h RS232CDllóp¥∏ΩŒﬂ∞ƒä÷êî ==============================================
// Open/Close/Send/Receive
// HANDLE OpenCommPort(CString strCommPort);
HANDLE OpenCommPort(char* str);
BOOL CloseCommPort(HANDLE hComm);
DWORD CommSendData(HANDLE hComm, char * lpszData, DWORD dwCount);
int CommReadBuffer(HANDLE hComm, void * lpszData, int iMaxSize, BOOL fDelete);
int CommGetReceivedSize(HANDLE hComm);

// êßå‰ê¸RTS/DTR ==============================================================
BOOL CommTglRts(HANDLE hComm);
BOOL CommSetRtsState(HANDLE hComm, DWORD dwStatus);
BOOL CommSetRts(HANDLE hComm);
BOOL CommClearRts(HANDLE hComm);
BOOL CommSetDtr(HANDLE hComm);
BOOL CommClearDtr(HANDLE hComm);
BOOL CommSetXoff(HANDLE hComm);
BOOL CommSetXon(HANDLE hComm);
BOOL CommSetBreak(HANDLE hComm);
BOOL CommClearBreak(HANDLE hComm);
DWORD CommSetBaudRate(HANDLE hComm, DWORD dwBaudRate);
DWORD CommGetBaudRate(HANDLE hComm);
BYTE CommSetParity(HANDLE hComm, BYTE bParity);
BYTE CommGetParity(HANDLE hComm);
BYTE CommSetStopBits(HANDLE hComm, BYTE bStopBits);
BYTE CommGetStopBits(HANDLE hComm);
DCB * CommGetDCB(HANDLE hComm);
// CString CommGetCommPort(HANDLE hComm);

// î˜í≤êÆóp ===================================================================
double CommGetWaitTime(HANDLE hComm);
double CommSetWaitTime(HANDLE hComm, double fWait);
