/*
 * test_fcntl.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"

#define INQ_REPLY_LEN 96
#define INQ_CMD_CODE 0x12
#define INQ_CMD_LEN 6


int main(int argc, char **argv)
{

	char *path = strcat(get_testfiles_dir(),"/test_icotl.txt");
	test_ioctl(path);
	return 0;
}


void test_ioctl(char *path)
{
	/* taken from http://tldp.org/HOWTO/SCSI-Generic-HOWTO/pexample.html */
    int sg_fd, k;
    unsigned char inqCmdBlk[INQ_CMD_LEN] =
                    {INQ_CMD_CODE, 0, 0, 0, INQ_REPLY_LEN, 0};
	/* This is a "standard" SCSI INQUIRY command. It is standard because the
	 * CMDDT and EVPD bits (in the second byte) are zero. All SCSI targets
	 * should respond promptly to a standard INQUIRY */
    unsigned char inqBuff[INQ_REPLY_LEN];
    unsigned char sense_buffer[32];
    sg_io_hdr_t io_hdr;

    if ((sg_fd = open(path, O_RDONLY)) < 0) {
    	/* Note that most SCSI commands require the O_RDWR flag to be set */
        fprintf(stderr, "error opening given file name");
        return;
    }

    /* It is prudent to check we have a sg device by trying an ioctl */
    if ((ioctl(sg_fd, SG_GET_VERSION_NUM, &k) < 0) || (k < 30000)) {
       fprintf(stdout, "%s is not an sg device, or old sg driver\n", path);
        return ;
    }
    /* Prepare INQUIRY command */

    memset(&io_hdr, 0, sizeof(sg_io_hdr_t));

    io_hdr.interface_id = 'S';
    io_hdr.cmd_len = sizeof(inqCmdBlk);
    /* io_hdr.iovec_count = 0; */  /* memset takes care of this */
    io_hdr.mx_sb_len = sizeof(sense_buffer);
    io_hdr.dxfer_direction = SG_DXFER_FROM_DEV;
    io_hdr.dxfer_len = INQ_REPLY_LEN;
    io_hdr.dxferp = inqBuff;
    io_hdr.cmdp = inqCmdBlk;
    io_hdr.sbp = sense_buffer;
    io_hdr.timeout = 20000;     /* 20000 millisecs == 20 seconds */

    /* io_hdr.flags = 0; */     /* take defaults: indirect IO, etc */
    /* io_hdr.pack_id = 0; */
    /* io_hdr.usr_ptr = NULL; */

    if (ioctl(sg_fd, SG_IO, &io_hdr) < 0) {
        fprintf(stderr, "sg_simple0: Inquiry SG_IO ioctl error \n");
        return;
    }

    /* now for the error processing */
    if ((io_hdr.info & SG_INFO_OK_MASK) != SG_INFO_OK) {
        if (io_hdr.sb_len_wr > 0) {
           fprintf(stdout, "INQUIRY sense data: ");
            for (k = 0; k < io_hdr.sb_len_wr; ++k) {
                if ((k > 0) && (0 == (k % 10)))
                   fprintf(stdout, "\n  ");
               fprintf(stdout, "0x%02x ", sense_buffer[k]);
            }
           fprintf(stdout, "\n");
        }
        if (io_hdr.masked_status)
           fprintf(stdout, "INQUIRY SCSI status=0x%x\n", io_hdr.status);
        if (io_hdr.host_status)
           fprintf(stdout, "INQUIRY host_status=0x%x\n", io_hdr.host_status);
        if (io_hdr.driver_status)
           fprintf(stdout, "INQUIRY driver_status=0x%x\n", io_hdr.driver_status);
    }
    else {  /* assume INQUIRY response is present */
        char * p = (char *)inqBuff;
       fprintf(stdout, "Some of the INQUIRY command's response:\n");
       fprintf(stdout, "    %.8s  %.16s  %.4s\n", p + 8, p + 16, p + 32);
       fprintf(stdout, "INQUIRY duration=%u millisecs, resid=%d\n",
               io_hdr.duration, io_hdr.resid);
    }


	if (close(sg_fd) != 0){
		fprintf(stderr, "close() error \n");
		return;
	}
}
