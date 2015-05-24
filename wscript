#!/usr/bin/env python
# encoding: utf-8

import os
from os import path

COMPILER_NAME = 'squirrel'

def options(opt):
    opt.load('compiler_c')
    
    opt.add_option('-e', '--example', help='Informa qual exemplo será compilado', dest='example_name')

def configure(conf):
    conf.load('compiler_c flex bison')
    #isto talvez só seja necessário no windows (?)
    conf.env.LIB_FLEX = ['fl']
    # muda o nome do arquivo ".h" para y.tab.h
    conf.env.BISONFLAGS = ['--defines=y.tab.h', '-v']

def build(bld):

    path_datastructs = path.join('libs', 'datastructs-c','src');
    bld.stlib(
        target          = 'datastructs-c',
        name            = 'datastructs-c',
        source          = bld.path.ant_glob(path.join(path_datastructs, '*.c')),
        cflags          = ['--std=gnu99'],
        includes        = path_datastructs,
        export_includes = path_datastructs
        )

    path_commons = path.join('src','commons');
    bld.stlib(
        target          = 'squirrel-commons',
        source          = bld.path.ant_glob(path.join(path_commons,'**', '*.c')),
        includes        = path_commons,
        export_includes = path_commons,
        use             = ['datastructs-c'])

    path_compiler = path.join('src','compiler');
    bld.stlib(
        target          = 'squirrel-compiler',
        source          = bld.path.ant_glob(path.join('src','compiler','**', '*.c')),
        includes        = path_compiler,
        export_includes = path_compiler,
        use             = ['datastructs-c', 'squirrel-commons'])

    bld(
        target   = COMPILER_NAME,
        features = 'c cprogram',
        source   = ['lexer.l','parser.y'],
        includes = ['.'],
        use      = ['FLEX', 'datastructs-c', 'squirrel-compiler', 'squirrel-commons'])

    
    if(bld.options.example_name):
        path_language = path.join('src','language');
        bld.stlib(
            target          = 'squirrel-language',
            source          = bld.path.ant_glob(path.join('src','language','**', '*.c')),
            includes        = path_language,
            export_includes = path_language,
            use             = ['datastructs-c', 'squirrel-commons'])
            
        bld.program(
            target  = bld.options.example_name,
            source  = path.join('traducoes_exemplo', bld.options.example_name + ".c"),
            use     = ['squirrel-language'])
        

from waflib.Build import BuildContext

# Criando comando especial 'run' para executar compilador
class RunContext(BuildContext):
        cmd = 'run'
        fun = 'run'
def run(ctx):
    ctx(rule=path.join(ctx.path.get_bld().abspath(), COMPILER_NAME));
    
    
