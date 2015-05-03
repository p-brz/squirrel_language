#!/usr/bin/env python
# encoding: utf-8

import os
from os import path
        
PARSER_NAME = 'squirrel' 

def options(opt):
	opt.load('compiler_c')

def configure(conf):
	conf.load('compiler_c flex bison')
	#isto talvez só seja necessário no windows (?)
	conf.env.LIB_FLEX = ['fl']
	# muda o nome do arquivo ".h" para y.tab.h
	conf.env.BISONFLAGS = ['--defines=y.tab.h', '-v']
    
def build(bld):
	bld(
		features = 'c cprogram',
		source = ['lexer.l','parser.y'],
		target = PARSER_NAME,
		use='FLEX')
		
# Criando comando especial 'run' para executar compilador
from waflib.Build import BuildContext
class RunContext(BuildContext):
        cmd = 'run'
        fun = 'run'
def run(ctx):
    ctx(rule=path.join(ctx.path.get_bld().abspath(), PARSER_NAME));
		
