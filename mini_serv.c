#include	<stdio.h>
#include	<unistd.h>
#include	<string.h>
#include	<stdlib.h>
#include	<arpa/inet.h>
#include	<sys/socket.h>
#include	<netinet/in.h>

// typedef struct server_t;

typedef struct
{
	int	_fd;
	char*	_name;		// Nome completo do cliente
	char*	_nick;		// Apelido do cliente
	unsigned int	_id;	// ID do cliente, o começa em zero
}	client_t;

typedef struct
{
	int	_fd;
	int	_port;
	char*	_pass;			// PassWord
	unsigned int	_n_client;	// Number of client
	client_t*	_client;		// Array/List/Set (Conjunto) de clients

}	server_t;

static int	setup( server_t*	server );
static int	only_digits( const char* );

int	main( int argc, char** argv )
{
	if ( argc != 3 )
	{
		const char* _error = "Wrong number of arguments";

		write( STDERR_FILENO, _error, strlen(_error) );
		write( STDERR_FILENO, "\n", 1);

		exit(1);
	}


	if ( !only_digits( argv[1] ) )
	{
		write(STDIN_FILENO, "ERROR", 5);
		write(STDIN_FILENO, "\n", 1);
	}

	server_t	server;

	server._port = atoi( argv[1] );
	server._pass = argv[2];
	server._n_client = 0;

	setup( &server );

	// printf( "PORT\t\t: %d\n", server._port );
	// printf( "PASSWORD\t: %s\n", server._pass );

	printf( "SOCKET FD\t: %d\n", server._fd );

	close( server._fd );

	return 0;
}

static int	only_digits( const char* _port )
{
	if ( _port == NULL )
		return 0;
	
	while ( *_port )	{
		if ( ( unsigned int )*_port < 48 || (unsigned int)*_port > 57 )
			return 0;
		_port++;
	}

	return 1;
}

static int	setup( server_t*	server )
{
	const char*	_error = "Fatal error\n";

	/*
	 *
	 *
	 *	socket()
	 * 
	 * 
	*/

	if ( ( server->_fd = socket(AF_INET, SOCK_STREAM, 0) ) == -1 )
	{
		write( STDERR_FILENO, _error, strlen(_error) );
		return 0;
	}

	/*
	 *
	 *	bind()
	 *
	 *
	 * 
	 * 
	 * */

	struct sockaddr_in	addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(server->_port);
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
	// addr.sin_zero = ;

	if ( bind(server->_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1 )
	{
		write( STDERR_FILENO, _error, strlen(_error) );
		close(server->_fd);
		return 0;
	}

	/**
	 * 
	 *	listen()
	 * 
	 * 
	 */

	if ( listen( server->_fd, SOMAXCONN ) == -1 )
	{
		write( STDERR_FILENO, _error, strlen(_error) );
		close(server->_fd);
		return 0;
	}

	return 1;
}