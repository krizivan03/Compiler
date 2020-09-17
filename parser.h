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
    struct monomial{
      int order = 0;
      int exponent = 1;
    };

    LexicalAnalyzer lexer;
    void ConsumeAllInput();
    void syntax_error();
    void parse_input();
    void parse_program();
    void parse_poly_decl_section();
    void parse_poly_decl();
    vector<string> parse_poly_header();
    vector<string> parse_id_list();
    void parse_poly_name();  
    void parse_poly_body();
    void parse_term_list();
    void parse_term();
    void parse_monomial_list();
    monomial *parse_monomial();
    int parse_exponent();
    void parse_add_operator();
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

    Token expect(TokenType expected_type);
};

#endif

