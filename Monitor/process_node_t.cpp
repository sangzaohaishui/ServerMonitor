#include "process_node_t.h"
#include "dahlia_utils.h"
#include "stringutil.h"
#include <atlstr.h>
#include "nbclock.h"


dahlia::process_node_t::process_node_t( 
    redis_client_t* redis_mgr_, 
    const string& spath_, 
    int pid_ ,
    bool timeout_reboot_,
    unsigned int check_time):

    _be_reload(true),
    _redis_mgr(redis_mgr_),
    _process_name(spath_),
    _pid(pid_),
    _timeout_reboot(timeout_reboot_),
    _heart_check_tick(check_time),
    _lastupdate_ticket(nbclock::now_us() / 1000.0),
    _state(STATE_UNKNUWN)
{

}

dahlia::process_node_t::~process_node_t( void )
{

}

void dahlia::process_node_t::update_time()
{
    _lastupdate_ticket = (nbclock::now_us() / 1000.0);
}

bool dahlia::process_node_t::check(int& mem)
{
    //������״̬
    if ( _process_name == ""||
        _pid == 0)
    {
        mem = -1;
        return false;
    }

    //ͨ������ID��ȡ���̾��
    HANDLE  handle = get_process_handle(_pid);
    if (INVALID_HANDLE_VALUE == handle)
    {
        mem = -1;
        return false;
    }

    //��ȡ�ڴ���Ϣ
    mem = get_process_memroy_handle(handle);

    int  ret = WaitForSingleObject(handle , 200);

    //����WAIT_TIMEOUTʱ���ʾ�ý��̻�����,
    //WAIT_OBJECT_0��ʾ�߳�Ϊ�Ѿ�֪ͨ��״̬
    //WAIT_FAILED ��ʾΪ���������Ч���
    CloseHandle(handle);

    if (WAIT_TIMEOUT == ret)
    {

        //�����̴����б����Ƿ��ж��Դ���
        return true;
    }

    mem = -1;
    return false;
}

//����
bool dahlia::process_node_t::reboot()
{
    //ɱ��ԭ�н���
    if(termate())
    {
        //�����½���
        if(start())
        {
            //�����ڻ���ע����Ϣ��ʱ���ظ���������
            _be_reload = false;
            _heart_check_tick = -1;//��ʶ����������
            return true;
        }
    }
   
    return false;
}

void dahlia::process_node_t::stop()
{
    termate();
    _be_reload = false;
}

bool dahlia::process_node_t::start()
{
    STARTUPINFO startup_info_;
    PROCESS_INFORMATION process_info_;
    ZeroMemory(&startup_info_, sizeof(STARTUPINFOA ));
    //ZeroMemory(&process_info_, sizeof(LPPROCESS_INFORMATION ));


    //int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)_process_name.data(), -1, NULL,0);
    //WCHAR * wszGBK = new WCHAR[len+1];
    //memset(wszGBK, 0, len * 2 + 2);
    //MultiByteToWideChar(CP_ACP, 0, (LPCSTR)_process_name.data(), -1, (LPWSTR)wszGBK, len);

    //LPWSTR sCmd = wszGBK;
    CString strC(/*ConvertGBKToUtf8*/(_process_name).c_str());
    CString swork_path_(get_path().c_str());
    //��������
    if(CreateProcess(
        NULL,
        strC.GetBuffer(),
        NULL,
        NULL,
        false,
        0,
        NULL,
        swork_path_.GetBuffer(),
        &startup_info_,
        &process_info_
        ))
    {
        _pid = process_info_.dwProcessId;
        CloseHandle(process_info_.hProcess);
        CloseHandle(process_info_.hThread);
        return true;
    }

    ////��������Ϣ�洢�����ݿ���
    //delete []wszGBK;
    return false;
}

bool dahlia::process_node_t::termate()
{
    //ɱ������
   // return kill_process(get_processname(), _pid);
    return kill_by_path(_process_name);
}

std::string dahlia::process_node_t::get_path()
{
    return _process_name.substr(0, _process_name.rfind('\\'));
}

std::string dahlia::process_node_t::get_processname()
{
    string sret = _process_name.substr(_process_name.rfind('\\') + 1, _process_name.size());
    return sret;
}

std::string dahlia::process_node_t::get_processname( const string& spath_ )
{
    string sret = spath_.substr(spath_.rfind('\\') + 1, spath_.size());
    return sret;
}

bool dahlia::process_node_t::kill_by_path( const string& spath_ )
{
    //ͨ���������ƻ�ȡ����pid
    set<int> pid_s;
    string s_process = get_processname (spath_);

    get_processes_by_pname(s_process, pid_s);
    
    set<int>::iterator it = pid_s.begin();
    for ( ; it != pid_s.end(); ++it)
    {
        //ֻ����ͬ·���µ�exe��ɾ��
        string sPath = "";
        if(get_exe_path_by_pid(*it, sPath) &&
            sPath == spath_)
        {
            kill_process(s_process, *it);
        }
    }

    return true;
}

bool dahlia::process_node_t::contins_assert_wnd()
{
    bool bSave = false;
    string s_assert_file = "";
    if (_state != STATE_ASSERTING)
    {
        bSave = true;
        s_assert_file = get_path();
        s_assert_file += "\\assert.bmp";
    }

    bool ret = contions_assert_wnds(_pid, bSave, s_assert_file);
  

    return ret;
}
