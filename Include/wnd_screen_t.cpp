#include "wnd_screen_t.h"
#include "stringutil.h"
wnd_screen_t::wnd_screen_t( void )
{
    _buff = NULL;//�������紫�������
    _buf_len = 0;//���ݳ���
    _h_dib = NULL;
    _hdc = NULL;
    _hmdc = NULL;
    _m_h_wnd = NULL;
    _hhh = NULL;
}

wnd_screen_t::~wnd_screen_t(void)
{
     _h_bmp_screen = (HBITMAP)SelectObject(_hmdc, _hold_bmp);

    if (_h_dib)
    {
        delete []_h_dib;
    }

    if (_buff)
    {
        delete[] _buff;
    }

    bool ret =  DeleteObject(_h_bmp_screen);
    ret =  DeleteObject(_hold_bmp);
    ret =  DeleteDC(_hmdc);
    ret =  DeleteDC(_hdc);

    if (_m_h_wnd)
    {
        ret =    DeleteDC(_hhh);
    }


}

void wnd_screen_t::init( HWND h_wnd_, int w, int h, int color_deep_ /*= 16*/ )
{
    if (h_wnd_ != 0)
    {
        _m_h_wnd = h_wnd_;
        _hhh = ::GetDC(_m_h_wnd);
    }

    _hdc = CreateDC(L"DISPLAY", NULL, NULL, NULL);//�����豸������
    _scr_width = GetDeviceCaps(_hdc, HORZRES);//��Ļ���
    _src_height = GetDeviceCaps(_hdc, VERTRES);//��Ļ�߶�
    _hmdc = CreateCompatibleDC(_hdc);//�����ڴ��豸������
    if (w == 0 && h == 0)
    {
        _h_bmp_screen = CreateCompatibleBitmap(_hdc, _scr_width, _src_height);//����ָ���豸���ݵ�λͼ
    }
    else
    {
        _h_bmp_screen = CreateCompatibleBitmap(_hdc, w, h);//����ָ���豸���ݵ�λͼ
        _scr_width = w;
        _src_height = h;
    }

    _hold_bmp = (HBITMAP) SelectObject(_hmdc, _h_bmp_screen);
    BITMAP bm;//bmp��ѹ����jpgѹ�����������紫��

    BITMAPINFOHEADER bi;//λͼ��Ϣͷ����С40�ֽ�
    GetObject(_h_bmp_screen, sizeof(bm), (LPSTR)&bm);//�õ�ָ��ͼ�ζ�����Ϣ

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bm.bmWidth;
    bi.biHeight = bm.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = color_deep_;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~ 31) / 8) * bi.biHeight;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    int ncolors = (1 << bi.biBitCount);
    if(ncolors > 256)
    {
        ncolors = 0;
    }

    _dw_len_1 = bi.biSize + ncolors * sizeof(RGBQUAD);
    _dw_len_2 = _dw_len_1 + bi.biSizeImage;
    _h_dib = new char[_dw_len_2];

    memcpy(_h_dib, &bi, sizeof(bi));

    //��������
    _buff = new char[bi.biSizeImage];
    memcpy(_buff, _h_dib + _dw_len_1, bi.biSizeImage);
    _buf_len = bi.biSizeImage;

    bool ret =  DeleteObject(_h_bmp_screen);
    ret =  DeleteObject(_hold_bmp);
}

void wnd_screen_t::save(const string& spath)
{
    BITMAPFILEHEADER bfh;
    bfh.bfType = ((WORD)('M'<<8)|'B');
    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 54 + _dw_len_2;
    bfh.bfOffBits = 54;
    std::wstring wpath = string2wstring(spath);
    HANDLE hFile = CreateFile(wpath.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    DWORD dwWrite;
    WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwWrite, NULL);
    WriteFile(hFile, _h_dib, bfh.bfSize, &dwWrite, NULL);
    CloseHandle(hFile);
}

void wnd_screen_t::set_screen(int x, int y, int w, int h )
{
    /*::SetDIBits(_hhh, _h_bmp_screen, _scr_width, _src_height, (LPBYTE)_h_dib +_dw_len_1, (LPBITMAPINFO)_h_dib, 
    (DWORD)DIB_RGB_COLORS);

    StretchBlt(_hhh, x, y, w, h, _hdc, 0, 0, _scr_width, _src_height, SRCCOPY);*/
    BitBlt(_hmdc, 0, 0, w, h, _hdc, x, y, SRCCOPY);

    ::GetDIBits(_hmdc, _h_bmp_screen, 0L, _src_height, (LPBYTE)_h_dib +_dw_len_1, (LPBITMAPINFO)_h_dib, 
        (DWORD)DIB_RGB_COLORS);
    bool ret =  DeleteObject(_h_bmp_screen);
}
