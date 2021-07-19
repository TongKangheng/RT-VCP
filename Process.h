#include <new>
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>

#include <strsafe.h>
#include <assert.h>

#include <ks.h>
#include <ksmedia.h>
#include <Dbt.h>
extern int flag;
extern int key;
extern int bf;//brightness flag
extern int sf;//save flag
class Process
{
private:
	RGBQUAD *pframe;
	UINT width;
	UINT height;
public:
	Process(RGBQUAD *p, UINT w, UINT h);
	void to_black_and_white();
	void nagation();
	void to_emboss();
	void smooth();
	void sharp();
	void dip();
	void sketch();
	void up();
	void Save();
};