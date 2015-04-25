#include "wserver.h"

#define PREFIX_DSCLOG_worker_register_request	DebugLog( __FILE__ , __LINE__ , 
#define NEWLINE_DSCLOG_worker_register_request
#include "IDL_worker_register_request.dsc.LOG.c"

#define PREFIX_DSCLOG_worker_register_response	DebugLog( __FILE__ , __LINE__ , 
#define NEWLINE_DSCLOG_worker_register_response
#include "IDL_worker_register_response.dsc.LOG.c"

#define PREFIX_DSCLOG_execute_program_request	DebugLog( __FILE__ , __LINE__ , 
#define NEWLINE_DSCLOG_execute_program_request
#include "IDL_execute_program_request.dsc.LOG.c"

#define PREFIX_DSCLOG_execute_program_response	DebugLog( __FILE__ , __LINE__ , 
#define NEWLINE_DSCLOG_execute_program_response
#include "IDL_execute_program_response.dsc.LOG.c"

#define PREFIX_DSCLOG_deploy_program_request	DebugLog( __FILE__ , __LINE__ , 
#define NEWLINE_DSCLOG_deploy_program_request
#include "IDL_deploy_program_request.dsc.LOG.c"

int app_ExecuteProgramRequest( struct ServerEnv *penv , struct SocketSession *psession , execute_program_request *p_req );
int app_DeployProgramRequest( struct ServerEnv *penv , struct SocketSession *psession );

int proto_WorkerRegisterRequest( struct ServerEnv *penv , struct SocketSession *psession )
{
	int				msg_len ;
	
	int				nret = 0 ;
	
	worker_register_request		req ;
	
	CleanSendBuffer( psession );
	
	/* test
	00000103WRQ     {"os":"RedHatEnterpriseLinux","main_version":"5.8","bits":"64","ip":"127.0.0.1","port":11124}
	*/
	DSCINIT_worker_register_request( & req );
	nret = app_WorkerRegisterRequest( penv , psession , & req ) ;
	if( nret )
	{
		ErrorLog( __FILE__ , __LINE__ , "app_WorkerRegisterRequest failed[%d]" , nret );
		return -1;
	}
	else
	{
		DebugLog( __FILE__ , __LINE__ , "app_WorkerRegisterRequest ok" );
	}
	
	DSCLOG_worker_register_request( & req );
	
	msg_len = psession->send_buffer_size-1 - LEN_COMMHEAD - LEN_MSGHEAD_MSGTYPE ;
	nret = DSCSERIALIZE_JSON_COMPACT_worker_register_request( & req , NULL , psession->send_buffer + LEN_COMMHEAD + LEN_MSGHEAD , & msg_len ) ;
	if( nret )
	{
		ErrorLog( __FILE__ , __LINE__ , "DSCSERIALIZE_JSON_COMPACT_worker_register_request failed[%d]" , nret );
		return -1;
	}
	else
	{
		DebugLog( __FILE__ , __LINE__ , "DSCSERIALIZE_JSON_COMPACT_worker_register_request ok , [%d]bytes" , msg_len );
	}
	
	FormatSendHead( psession , "WRQ" , msg_len );
	
	InfoLog( __FILE__ , __LINE__ , "output buffer [%d]bytes[%.*s]" , psession->total_send_len - LEN_COMMHEAD , psession->total_send_len - LEN_COMMHEAD , psession->send_buffer + LEN_COMMHEAD );
	
	return 0;
}

int proto_DeployProgramRequest( struct ServerEnv *penv , struct SocketSession *psession , execute_program_request *p_req )
{
	int				msg_len ;
	
	int				nret = 0 ;
	
	deploy_program_request		req ;
	
	CleanSendBuffer( psession );
	
	/* test
	00000103PEQ     {"status":0}
	*/
	DSCINIT_deploy_program_request( & req );
	strcpy( req.program , p_req->program );
	
	DSCLOG_deploy_program_request( & req );
	
	msg_len = psession->send_buffer_size-1 - LEN_COMMHEAD - LEN_MSGHEAD_MSGTYPE ;
	nret = DSCSERIALIZE_JSON_COMPACT_deploy_program_request( & req , NULL , psession->send_buffer + LEN_COMMHEAD + LEN_MSGHEAD , & msg_len ) ;
	if( nret )
	{
		ErrorLog( __FILE__ , __LINE__ , "DSCSERIALIZE_JSON_COMPACT_deploy_program_request failed[%d]" , nret );
		return -1;
	}
	else
	{
		DebugLog( __FILE__ , __LINE__ , "DSCSERIALIZE_JSON_COMPACT_deploy_program_request ok , [%d]bytes" , msg_len );
	}
	
	FormatSendHead( psession , "DPQ" , msg_len );
	
	// InfoLog( __FILE__ , __LINE__ , "output buffer [%d]bytes[%.*s]" , psession->total_send_len - LEN_COMMHEAD , psession->total_send_len - LEN_COMMHEAD , psession->send_buffer + LEN_COMMHEAD );
	
	return 0;
}

int proto_ExecuteProgramResponse( struct ServerEnv *penv , struct SocketSession *psession , int response_code , int status )
{
	int				msg_len ;
	
	execute_program_response	req ;
	
	int				nret = 0 ;
	
	CleanSendBuffer( psession );
	
	DSCINIT_execute_program_response( & req );
	req.response_code = response_code ;
	req.status = status ;
	
	DSCLOG_execute_program_response( & req );
	
	msg_len = psession->send_buffer_size-1 - LEN_COMMHEAD - LEN_MSGHEAD_MSGTYPE ;
	nret = DSCSERIALIZE_JSON_COMPACT_execute_program_response( & req , NULL , psession->send_buffer + LEN_COMMHEAD + LEN_MSGHEAD , & msg_len ) ;
	if( nret )
	{
		ErrorLog( __FILE__ , __LINE__ , "DSCSERIALIZE_JSON_COMPACT_execute_program_response failed[%d]" , nret );
		return -1;
	}
	else
	{
		DebugLog( __FILE__ , __LINE__ , "DSCSERIALIZE_JSON_COMPACT_execute_program_response ok , [%d]bytes" , msg_len );
	}
	
	FormatSendHead( psession , "EPP" , msg_len );
	
	InfoLog( __FILE__ , __LINE__ , "output buffer [%d]bytes[%.*s]" , psession->total_send_len - LEN_COMMHEAD , psession->total_send_len - LEN_COMMHEAD , psession->send_buffer + LEN_COMMHEAD );
	
	return 0;
}

funcDoProtocol proto ;
int proto( void *_penv , struct SocketSession *psession )
{
	struct ServerEnv	*penv = (struct ServerEnv *) _penv ;
	int			msg_len ;
	
	int			nret = 0 ;
	
	CleanSendBuffer( psession );
	
	InfoLog( __FILE__ , __LINE__ , "input buffer [%d]bytes[%.*s]" , psession->recv_body_len , psession->recv_body_len , psession->recv_buffer + LEN_COMMHEAD );
	
	if( psession->recv_body_len < LEN_COMMHEAD )
	{
		ErrorLog( __FILE__ , __LINE__ , "body data is too short" );
		return -1;
	}
	
	if( STRNCMP( psession->recv_buffer + LEN_COMMHEAD , == , "WRP" , LEN_MSGHEAD_MSGTYPE ) )
	{
		worker_register_response	rsp ;
		
		/* test
		00000 29WRP     {"response_code":0}
		*/
		msg_len = psession->total_recv_len - LEN_COMMHEAD - LEN_MSGHEAD_MSGTYPE ;
		DSCINIT_worker_register_response( & rsp );
		nret = DSCDESERIALIZE_JSON_COMPACT_worker_register_response( NULL , psession->recv_buffer + LEN_COMMHEAD + LEN_MSGHEAD , & msg_len , & rsp ) ;
		if( nret )
		{
			ErrorLog( __FILE__ , __LINE__ , "DSCDESERIALIZE_JSON_COMPACT_worker_register_response failed[%d]" , nret );
			return -1;
		}
		else
		{
			DebugLog( __FILE__ , __LINE__ , "DSCDESERIALIZE_JSON_COMPACT_worker_register_response ok" );
		}
		
		DSCLOG_worker_register_response( & rsp );
		
		nret = app_WorkerRegisterResponse( penv , psession , & rsp ) ;
		if( nret )
		{
			ErrorLog( __FILE__ , __LINE__ , "app_WorkerRegisterRequest failed[%d]" , nret );
			return -1;
		}
	}
	else if( STRNCMP( psession->recv_buffer + LEN_COMMHEAD , == , "EPQ" , LEN_MSGHEAD_MSGTYPE ) )
	{
		execute_program_request		req ;
		execute_program_response	rsp ;
		
		/* test
		00000082DEQ     {"program":"shixin_person_downloadlist","md5":"ABCDEFG","exec_param":""}
		*/
		
		DSCINIT_execute_program_request( & req );
		DSCINIT_execute_program_response( & rsp );
		
		msg_len = psession->total_recv_len - LEN_COMMHEAD - LEN_MSGHEAD_MSGTYPE ;
		nret = DSCDESERIALIZE_JSON_COMPACT_execute_program_request( NULL , psession->recv_buffer + LEN_COMMHEAD + LEN_MSGHEAD , & msg_len , & req ) ;
		if( nret )
		{
			ErrorLog( __FILE__ , __LINE__ , "DSCDESERIALIZE_JSON_COMPACT_execute_program_request failed[%d]" , nret );
			return -1;
		}
		else
		{
			DebugLog( __FILE__ , __LINE__ , "DSCDESERIALIZE_JSON_COMPACT_execute_program_request ok" );
		}
		
		DSCLOG_execute_program_request( & req );
		
		nret = app_ExecuteProgramRequest( penv , psession , & req ) ;
		if( nret )
		{
			ErrorLog( __FILE__ , __LINE__ , "app_WorkerRegisterRequest failed[%d]" , nret );
			return -1;
		}
	}
	else if( STRNCMP( psession->recv_buffer + LEN_COMMHEAD , == , "DPQ" , LEN_MSGHEAD_MSGTYPE ) )
	{
		nret = app_DeployProgramRequest( penv , psession ) ;
		if( nret )
		{
			ErrorLog( __FILE__ , __LINE__ , "app_DeployProgramRequest failed[%d]" , nret );
			return -1;
		}
	}
	else
	{
		ErrorLog( __FILE__ , __LINE__ , "unknow msgtype[%.3s]" , psession->recv_buffer + LEN_COMMHEAD );
		return -1;
	}
	
	if( psession->total_send_len )
	{
		InfoLog( __FILE__ , __LINE__ , "output buffer [%d]bytes[%.*s]" , psession->total_send_len - LEN_COMMHEAD , psession->total_send_len - LEN_COMMHEAD , psession->send_buffer + LEN_COMMHEAD );
	}
	
	return 0;
}