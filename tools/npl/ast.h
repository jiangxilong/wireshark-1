typedef enum {
	OP1_INVALID = 0,
	OP1_MINUS,
	OP1_NOT,
	OP1_NEG
} npl_op1_t;

typedef enum {
	OP2_INVALID = 0,

	OP2_ASSIGN,

	OP2_PLUS,
	OP2_MINUS,

	OP2_MULTIPLY,
	OP2_DIV,
	OP2_MOD,

	OP2_SHL,
	OP2_SHR,

	OP2_EQUAL,
	OP2_NOTEQUAL,

	OP2_LESS,
	OP2_GREATER,
	OP2_LEQUAL,
	OP2_GEQUAL,

	OP2_LOGIC_OR,
	OP2_LOGIC_AND,

	OP2_OR,
	OP2_AND,
	OP2_XOR,

} npl_op2_t;

#define NPL_PARAMS_MAX 20

typedef struct {
	char *args[NPL_PARAMS_MAX];
	int count;

} npl_params_t;

typedef enum {
	EXPRESSION_INVALID = 0,

	EXPRESSION_ID,
	EXPRESSION_INT,
	EXPRESSION_STR,

	EXPRESSION_INDEX,
	EXPRESSION_FIELD,
	EXPRESSION_CALL,

	EXPRESSION_UNARY,
	EXPRESSION_BINARY,
	EXPRESSION_COND

} npl_expression_type_t;

typedef struct _npl_expression {
union {
	struct {
		npl_expression_type_t type;
	};

	struct {
		npl_expression_type_t type;	/* EXPRESSION_ID */
		char *id;
	} id;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_INT */
		unsigned int digit;
	} num;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_STR */
		char *str;
	} str;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_INDEX */

		struct _npl_expression *base;
		struct _npl_expression *index;
	} arr;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_FIELD */

		struct _npl_expression *base;
		struct _npl_expression *field;
	} fld;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_UNARY */

		npl_op1_t operator;
		struct _npl_expression *operand;

	} u;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_BINARY */

		struct _npl_expression *operand1;
		struct _npl_expression *operand2;
		npl_op2_t operator;

	} b;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_CALL */

		struct _npl_expression *fn;

		struct _npl_expression_list {
			struct _npl_expression_list *next;

			struct _npl_expression *expr;

		} *args;

	} call;

	struct {
		npl_expression_type_t type;	/* EXPRESSION_COND */

		struct _npl_expression *test_expr;
		struct _npl_expression *true_expr;
		struct _npl_expression *false_expr;
	} c;


};
} npl_expression_t;

struct _npl_statement;

typedef struct {
	char *id;
	int private;
	npl_params_t params;

	npl_expression_t *format;
	npl_expression_t *count_expr;
	struct _npl_statements *sts;

} npl_struct_t;

typedef struct {
	char *id;
	npl_params_t params;

	npl_expression_t *switch_expr;

	struct npl_table_case {
		struct npl_table_case *next;

		npl_expression_t e;

		npl_expression_t *return_expr;

	} *cases;

	npl_expression_t *default_expr;

} npl_table_t;

typedef struct {
	char *id;
	npl_expression_t expr;

} npl_const_t;

typedef enum {
	STATEMENT_INVALID = 0,

	STATEMENT_WHILE,
	STATEMENT_TABLE,
	STATEMENT_STRUCT,
	STATEMENT_FIELD,
	STATEMENT_SWITCH,
	STATEMENT_DYNAMIC_SWITCH,

} npl_statement_type_t;

typedef struct {
	npl_expression_t *switch_expr;

	struct npl_switch_case {
		struct npl_switch_case *next;

		npl_expression_t e;

		struct _npl_statement *st;

	} *cases;

	struct _npl_statement *default_st;

} npl_switch_t;

typedef struct _npl_statement {
	union {
		struct {
			npl_statement_type_t type;
		};

		struct {
			npl_statement_type_t type;	/* STATEMENT_WHILE */

			char *id;
			npl_expression_t expr;

			struct _npl_statements *sts;
		} w;

		struct {
			npl_statement_type_t type;	/* STATEMTN_TABLE */

			npl_table_t data;
		} t;

		struct {
			npl_statement_type_t type;	/* STATEMENT_STRUCT */

			npl_struct_t data;
		} s;

		struct {
			npl_statement_type_t type;	/* STATEMENT_SWITCH or STATEMENT_DYNAMIC_SWITCH */

			npl_switch_t data;
		} sw;

		struct {
			npl_statement_type_t type;	/* STATEMENT_FIELD */
			
			char *t_id;
			char *id;

			unsigned int bits;
			npl_expression_t *arr;

			npl_expression_t *format;
			struct _npl_statements *sts;

			struct hfinfo *hfi; /* set by code gen */
		} f;

	};
} npl_statement_t;

struct _npl_statements {
	struct _npl_statements *next;

	npl_statement_t st;
};

typedef struct {
	char *id;
	npl_params_t params;

	npl_expression_t *format;
	struct _npl_statements *sts;

} npl_protocol_t;

typedef enum {
	FIELD_INVALID = 0,

	FIELD_DECIMAL,
	FIELD_NUMBER,
	FIELD_TIME,
	FIELD_UNSIGNED_NUMBER

} npl_field_type_t;

typedef struct {
	npl_field_type_t type;

	char *name;
	npl_params_t params;

	npl_expression_t *byte_order;
	npl_expression_t *display_format;
	npl_expression_t *size;

} npl_type_t;

typedef enum {
	DECL_INVALID = 0,

	DECL_ATTR,
	DECL_STRUCT,
	DECL_TABLE,
	DECL_CONST,
	DECL_PROTOCOL,
	DECL_TYPE

} npl_decl_type_t;

typedef struct _npl_attr {
	npl_expression_t expr;
} npl_attr_t;

typedef struct {
	union {
		struct {
			npl_decl_type_t type;
		};

		struct {
			npl_decl_type_t type;	/* DECL_ATTR */

			npl_attr_t data;
		} a;

		struct {
			npl_decl_type_t type;	/* DECL_STRUCT */

			npl_struct_t data;
		} s;

		struct {
			npl_decl_type_t type;	/* DECL_TABLE */

			npl_table_t data;
		} t;

		struct {
			npl_decl_type_t type;	/* DECL_PROTOCOL */

			npl_protocol_t data;
		} p;

		struct {
			npl_decl_type_t type;	/* DECL_CONST */

			npl_const_t data;
		} c;

		struct {
			npl_decl_type_t type;	/* DECL_TYPE */

			npl_type_t data;
		} ty;

	};
} npl_decl_t;

typedef struct {
	struct _npl_decl_list {
		struct _npl_decl_list *next;
		npl_decl_t d;

	} *decls;

	int parse_ok;

} npl_code_t;
