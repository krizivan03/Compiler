/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include <map>
#include <vector>
#include "lexer.h"

using namespace std;
class Parser {
  public:
    typedef struct monomial{
      int order = 0;
      int exponent = 1;
      monomial * next = NULL;
    }monomial;
    typedef struct term{
      int coefficient;
      TokenType addOperator;
      monomial * head_monomial;
      term * next = NULL;
    }term;
    typedef struct param_ID{
      string ID;
      int order;
      param_ID * next = NULL;
    }param_ID;
    typedef struct poly_dec{
      string name;
      param_ID * theparam_IDs;
      term * terms;
      poly_dec * next_poly_dec = NULL;
    }poly_dec;
    /*----------------------------------------------*/
    
    typedef struct arg{
      TokenType arg_type; // 0 NUM , 1 ID, 2 poly_eval
      int value;
      int index;
      struct poly_eval *p = NULL;
    }arg;
    typedef struct poly_eval{
      poly_dec *apoly_dec;
      arg *theArgs;
    }poly_eval;

    typedef struct stmt{
      int stmt_type = 0; // 0 INPUT , 1 POLY EVAL
      poly_eval *p = NULL;
      int variable;
      stmt* next;
    }stmt;
    
    

    LexicalAnalyzer lexer;
    void ConsumeAllInput();
    void syntax_error();
    void parse_input();
    void parse_program();
    poly_dec *parse_poly_decl_section();
    poly_dec *parse_poly_decl();
    param_ID* parse_poly_header();
    param_ID* parse_id_list(int order);
    string parse_poly_name();  
    term * parse_poly_body();
    term *parse_term_list();
    term *parse_term();
    monomial *parse_monomial_list();
    monomial *parse_monomial();
    int parse_exponent();
    TokenType parse_add_operator();
    int parse_coefficient(); 
    void parse_START();
    void parse_inputs();
    void parse_statement_list();
    void parse_statement();
    void parse_poly_eval_statement();
    void parse_input_statement();
    void parse_poly_eval();
    void parse_arg_list();
    void parse_arg();
    void allocateMem(Token t);
    void storeInput(Token t);
    void getOrder(poly_dec *apoly_dec);
    Token expect(TokenType expected_type);
};

#endif

