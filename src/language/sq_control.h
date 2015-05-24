#ifndef SQ_CONTROL_H
#define SQ_CONTROL_H


/****************************** FOR MACROS ********************************/

#define FOR_START(for_id, initialization, cond_test)\
    initialization;             \
    /* O operador ## concatena for_test_ com o valor de for_id*/ \
    for_test_##for_id :         \
    if(!(cond_test))            \
       goto for_exit_##for_id ;
        
#define FOR_END(for_id, incr_stmt)      \
    incr_stmt;                      \
    goto for_test_##for_id;         \
    for_exit_##for_id:              \
    ; //empty statement

/****************************** IF-ELSE MACROS ********************************/

#define IF_START(if_id, condition) \
    if(!(condition))               \
        goto if_exit_##if_id ;

#define IF_END(if_id)   \
    if_exit_##if_id:     \
        ;

#define ELSE_START(if_id)       \
    /*Esta instrução vai ser executada se o if for executado*/\
    goto else_exit_##if_id ;    \
    if_exit_##if_id:            \
        ;                       \

#define ELSE_END(if_id) \
    else_exit_##if_id : \
        ;             

/******************************************************************************/

#endif
