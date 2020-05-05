/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "remoteproc.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

float *
meshtimh_1(data *argp, CLIENT *clnt)
{
	static float clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, MeshTimh,
		(xdrproc_t) xdr_data, (caddr_t) argp,
		(xdrproc_t) xdr_float, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

result1 *
maxmin_1(data *argp, CLIENT *clnt)
{
	static result1 clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, MaxMin,
		(xdrproc_t) xdr_data, (caddr_t) argp,
		(xdrproc_t) xdr_result1, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

new_array *
powerarray_1(argument1 *argp, CLIENT *clnt)
{
	static new_array clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, PowerArray,
		(xdrproc_t) xdr_argument1, (caddr_t) argp,
		(xdrproc_t) xdr_new_array, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
