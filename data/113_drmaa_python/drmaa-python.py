import drmaa
import os

def main():
    """
    Submit a job.
    """
    with drmaa.Session() as s:
        print('Creating job template')
        jt = s.createJobTemplate()
        jt.remoteCommand = 'cd $PBS_O_WORKDIR ; echo PWD=$PWD'
        jt.nativeSpecification = "-v PBS_O_WORKDIR="+os.getcwd()+",project=resv -l select=1:ncpus=1:mem=10mb -N drmaa_python -l walltime=0:01:00"
        jobid = s.runJob(jt)
        print('%s' % jobid)

if __name__=='__main__':
    main()
