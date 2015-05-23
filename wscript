#!/usr/bin/env python
# encoding: utf-8

import os
from os import path

PARSER_NAME = 'squirrel'

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
    datastructsDir = path.join(*['libs', 'datastructs-c', 'src']);
    print(datastructsDir)
    sources = bld.path.ant_glob(path.join(datastructsDir, '*.c'));

    print(sources)

    bld.stlib(
        target= 'datastructs-c',
        name= 'datastructs-c',
        source = sources,
        cflags = ['--std=gnu99'],
        includes = datastructsDir,
        export_includes = datastructsDir
        )

    bld.stlib(
        target='squirrel-lib',
        source = bld.path.ant_glob('src/**/*.c'),
        includes = ['src'],
        export_includes = ['src'],
        use=['datastructs-c'])

    bld(
        features = 'c cprogram',
        source = ['lexer.l','parser.y'],
        includes = ['src', '.'],
        target = PARSER_NAME,
        use=['FLEX', 'datastructs-c', 'squirrel-lib'])

    
    if(bld.options.example_name):
        bld.stlib(
            target='sqlib',
            source = bld.path.ant_glob('language/**/*.c'),
            includes = ['language'],
            export_includes = ['language'],
            use=['datastructs-c'])
            
        bld.program(
            target=bld.options.example_name,
            source=path.join('traducoes_exemplo', bld.options.example_name + ".c"),
            use='sqlib')
        

from waflib.Build import BuildContext

# Criando comando especial 'run' para executar compilador
class RunContext(BuildContext):
        cmd = 'run'
        fun = 'run'
def run(ctx):
    ctx(rule=path.join(ctx.path.get_bld().abspath(), PARSER_NAME));
    
    
