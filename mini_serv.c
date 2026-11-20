#include	<stdio.h>
#include	<unistd.h>
#include	<string.h>
#include	<stdlib.h>
#include	<arpa/inet.h>
#include	<sys/socket.h>
#include	<netinet/in.h>
#include	<sys/select.h>

// typedef struct server_t;

typedef struct
{
	int	_fd;
	char*	_buffer;
	unsigned int	_id;	// ID do cliente, o começa em zero
}	client_t;

typedef struct
{
	int	_fd;
	int	_port;
	unsigned int	_nfds;	// Number of client
	client_t**	_client;		// Array/List/Set (Conjunto) de clients

}	server_t;

static int	max_fd( server_t* server );
static void	loop( server_t*	server );
static int	setup( server_t*	server );
static int	only_digits( const char* );

int	main( int argc, char** argv )
{
	if ( argc != 2 )
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
	server._nfds = 0;

	if ( setup( &server ) )
	{
		const char*	_error = "Fatal error\n";
		write( STDERR_FILENO, _error, strlen(_error) );
		return (1);
	}
	loop( &server );

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
	/*
	 *
	 *
	 *	socket()
	 * 
	 * 
	*/

	// FT_SETSIZE: 
	server->_client = ( client_t** ) malloc(( FD_SETSIZE + 1 ) * sizeof(client_t*) );
	if ( !server->_client )
	{
		close(server->_fd);
		return (1);
	}
	server->_client[ FD_SETSIZE ] = NULL;

	if ( ( server->_fd = socket(AF_INET, SOCK_STREAM, 0) ) == -1 )
	{
		close(server->_fd);
		return (1);
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
		close(server->_fd);
		return (1);
	}

	/**
	 * 
	 *	listen()
	 * 
	 * 
	 */

	if ( listen( server->_fd, SOMAXCONN ) == -1 )
	{
		close(server->_fd);
		return (1);
	}

	return 0;
}

static void	loop( server_t*	server )
{
	/**
	 * 
	 * • FD_ZERO(&set): limpa o conjunto (zera todos os bits ou a contagem).
	 * • FD_SET(fd, &set): adiciona o descritor ou socket fd ao conjunto.
	 * • FD_CLR(fd, &set): remove fd do conjunto.
	 * • FD_ISSET(fd, &set): devolve valor diferente de zero se fd estiver presente/pronto 
	 * 
	 */

	fd_set _allfds;
	// fd_set _exceptfds;
	// struct timeval _timout;

	FD_ZERO( &_allfds );
	FD_SET( server->_fd, &_allfds );
	
	while ( 1 )
	{
		fd_set _readfds	= _allfds;

		/**
		 * 
		 * 
		 * select()
		 * 
		 * 
		 * 
		 */

		
		int	_s = 0;
		int	_max = max_fd( server );

		printf("_nfds: %d\n", _max );
		if ( ( _s = select( _max + 1, &_readfds, NULL, NULL, NULL) ) < 0 )
		{
			write( STDERR_FILENO, "Fatal error\n", 12 );
			return ;
		}

		if ( _s == 0 )
			continue;

		if ( FD_ISSET(server->_fd, &_readfds) )
		{
			client_t*	client = (client_t *) malloc( sizeof(client_t) );
			if ( !client )
			{
				write( STDERR_FILENO, "Fatal error\n", 12 );
				return ;
			};
			if ( (client->_fd = accept( server->_fd, NULL, NULL )) == -1 )
			{
				free( client );
				continue;
			}
			client->_id = server->_nfds;
			server->_client[ server->_nfds ] = client;
			server->_nfds++;
			printf("client %d n: %d\n", client->_id, server->_nfds);
		}

		// while ()
		// {

		// }
	}
}

static int	max_fd( server_t* server )
{
	int	n = -1;
	int	_max = server->_fd;

	while ( ++n < (int) server->_nfds )
	{
		if ( _max < server->_client[n]->_fd )
			_max = server->_client[n]->_fd;
	}

	return _max;
}

// void	accepting()
// {

// }

// void	readble( server_t* server, fd_set* _readfds )
// {
// 	while ()
// 	{

// 	}
// }

// void	writeble( server_t* server, fd_set* _writefds )
// {
// 	while ()
// 	{

// 	}
// }