! <testinfo>
! test_generator=config/mercurium-fortran
! </testinfo>
PROGRAM P
IMPLICIT NONE
    CHARACTER*80  ERRMSG

    CHARACTER*9, PARAMETER :: FILENAME0 = 'FILENAME0'

    CHARACTER*9 FILENAME1
    PARAMETER (FILENAME1 = 'FILENAME1')

    CHARACTER(*) FILENAME2
    PARAMETER (FILENAME2 = 'FILENAME2')

    ERRMSG = FILENAME0 // ': BLA BLA'
    PRINT *, ERRMSG

    ERRMSG = FILENAME1 // ': BLA BLA'
    PRINT *, ERRMSG

    ERRMSG = FILENAME2 // ': BLA BLA'
    PRINT *, ERRMSG
END PROGRAM P