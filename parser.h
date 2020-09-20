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
      Token theToken;
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
      Token theToken;
      param_ID * theparam_IDs;
      term * terms;
      poly_dec * next_poly_dec = NULL;
    }poly_dec;
    /*----------------------------------------------*/
    struct poly_eval;
    typedef struct arg{
      TokenType arg_type; // NUM , ID, POLY (for poly_eval)
      int value = -3232; // if NUM
      int index = -3232;
      poly_eval *p = NULL;
      arg* next;
    }arg;
    typedef struct poly_eval{
      // poly_dec *apoly_dec;
      string poly_name;
      arg *theArgs;
    }poly_eval;

    typedef struct stmt{
      int stmt_type = 0; // 0 INPUT , 1 POLY EVAL
      poly_eval *p = NULL;
      int variable = -3232;
      string poly_name;
      stmt* next;
    }stmt;
    
    

    LexicalAnalyzer lexer;
    void ConsumeAllInput();
    void syntax_error();
    void Error_Code1();
    void Error_Code2();
    void Error_Code3();
    void Error_Code4();
    void Error_Code5();
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
    stmt *parse_statement_list();
    stmt *parse_statement();
    poly_eval * parse_poly_eval_statement();
    Token parse_input_statement();
    poly_eval *parse_poly_eval();
    arg * parse_arg_list();
    arg * parse_arg();
    void allocateMem(Token t);
    void storeInput(Token t);
    void getOrder(poly_dec *apoly_dec);
    Token expect(TokenType expected_type);
};

#endif

