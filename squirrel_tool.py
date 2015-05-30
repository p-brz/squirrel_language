#!/usr/bin/env python
# encoding: utf-8

from waflib.Task import Task
class sqToC(Task):
    run_str = '${SRC[0].abspath()} < ${SRC[1].abspath()} -o ${TGT}'
    color   = 'BROWN'

from waflib.TaskGen import extension

@extension('.sq')
def process_src(self, node):
    tg = self.bld.get_tgen_by_name('squirrel') 
    comp = tg.link_task.outputs[0]
    tsk = self.create_task('sqToC', [comp, node], node.change_ext('.c'))
    self.source.extend(tsk.outputs)