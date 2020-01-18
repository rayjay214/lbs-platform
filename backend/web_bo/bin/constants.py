from enum import IntEnum

class ErrCode(IntEnum):
    ErrOK = 0
    ErrDataNotFound = 1
    ErrLackParam = 2
    ErrPwdNotMatch = 3
    ErrTokenTimeout = 4
    ErrTokenInvalid = 5
    ErrMysqlError = 6


ErrMsg = {
    0 : 'OK',
    1 : 'Data not found',
    2 : 'lack parameter',
    3 : 'pwd not match',
    4 : 'token timeout',
    5 : 'token invalid',
    6 : 'mysql error'
}
