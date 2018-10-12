#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stg/plus_int/static.h"
#include "stg/bindings.h"
#include "containers/mmanager.h"
#include "stg/heap.h"
#include "data/string_.h"
ref var_41(ref thunk_ref);
ref var_39(ref thunk_ref);
ref var_21_cont(hash_map* bindings);
ref var_17_cont(hash_map* bindings);
ref var_16(ref thunk_ref);
ref var_14(ref thunk_ref);
ref var_12(ref thunk_ref);
ref var_10(ref thunk_ref);
ref var_7(ref thunk_ref);
ref var_3_cont(hash_map* bindings);
ref var_2_cont(hash_map* bindings);
ref var_1_cont(hash_map* bindings);
ref var_0_cont(hash_map* bindings);
ref main__slow(ref null);
ref main_();
void init_function_main_();
ref map_slow(ref null);
ref map(ref ma_f,  ref ma_l);
void init_function_map();
ref tail_slow(ref null);
ref tail(ref ta_l);
void init_function_tail();
ref head_slow(ref null);
ref head(ref he_l);
void init_function_head();
ref plus_int_slow(ref null);
ref plus_int(ref x1,  ref y1);
void init_function_plus_int();
void init_constructors_Unit();
void init_constructors_List();
void init_constructors_Int();
info_table plus_int_info_table;
ref plus_int_ref;
info_table head_info_table;
ref head_ref;
info_table tail_info_table;
ref tail_ref;
info_table map_info_table;
ref map_ref;
struct info_table I_info_table;
typedef struct I {
	info_table* info_ptr;
	int I_value;
} I;
struct info_table Cons_info_table;
typedef struct Cons {
	info_table* info_ptr;
	ref C_element;
	ref C_next;
} Cons;
struct info_table Nil_info_table;
typedef struct Nil {
	info_table* info_ptr;
} Nil;
struct info_table Unit_info_table;
typedef struct Unit {
	info_table* info_ptr;
} Unit;
void init_constructors_Int()
{
	I_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 1, .con_num = 0,  .con_name = (string_){.char_arr = "I",  .length = 1}}}};
}
void init_constructors_List()
{
	Cons_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 2, .con_num = 0,  .con_name = (string_){.char_arr = "Cons",  .length = 4}}}};
	Nil_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 0, .con_num = 1,  .con_name = (string_){.char_arr = "Nil",  .length = 3}}}};
}
void init_constructors_Unit()
{
	Unit_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 0, .con_num = 0,  .con_name = (string_){.char_arr = "Unit",  .length = 4}}}};
}
ref var_0_cont(hash_map* bindings)
{
	GET_BINDING(x1_ref, void**, x1, 0, bindings)
	info_table* x1_info = *((info_table**)x1);
	int var_18 = 0;
	if(x1_info->type == 1)
	{
		if((((x1_info)->extra).constructor).con_num == 0)
		{
			I* var_19 = (I*)x1;
			NEW_REF(a1_ref, int*, sizeof(int), a1)
			*(a1) = (var_19)->I_value;
			put_binding(bindings, 20,  a1_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_21_cont;
		}
	}
	else
	{
		assert((x1_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"xorq %%rcx, %%rcx\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(x1_ref), "r"(var_0_cont), "r"(bindings), "r"(var_18),  "r"(x1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_1_cont(hash_map* bindings)
{
	GET_BINDING(he_l_ref, void**, he_l, 0, bindings)
	info_table* he_l_info = *((info_table**)he_l);
	int var_22 = 0;
	if(he_l_info->type == 1)
	{
		if((((he_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_23 = (Cons*)he_l;
			ref he_v_ref = (var_23)->C_element;
			put_binding(bindings, 24,  he_v_ref);
			ref he_n_ref = (var_23)->C_next;
			put_binding(bindings, 25,  he_n_ref);
			return he_v_ref;
		}
		if((((he_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_26 = (Nil*)he_l;
			assert(false);
			ref var_27;
			return var_27;
		}
	}
	else
	{
		assert((he_l_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"xorq %%rcx, %%rcx\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(he_l_ref), "r"(var_1_cont), "r"(bindings), "r"(var_22),  "r"(he_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_2_cont(hash_map* bindings)
{
	GET_BINDING(ta_l_ref, void**, ta_l, 0, bindings)
	info_table* ta_l_info = *((info_table**)ta_l);
	int var_28 = 0;
	if(ta_l_info->type == 1)
	{
		if((((ta_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_29 = (Cons*)ta_l;
			ref ta_v_ref = (var_29)->C_element;
			put_binding(bindings, 30,  ta_v_ref);
			ref ta_n_ref = (var_29)->C_next;
			put_binding(bindings, 31,  ta_n_ref);
			return ta_n_ref;
		}
		if((((ta_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_32 = (Nil*)ta_l;
			assert(false);
			ref var_33;
			return var_33;
		}
	}
	else
	{
		assert((ta_l_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"xorq %%rcx, %%rcx\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(ta_l_ref), "r"(var_2_cont), "r"(bindings), "r"(var_28),  "r"(ta_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_3_cont(hash_map* bindings)
{
	GET_BINDING(ma_l_ref, void**, ma_l, 1, bindings)
	info_table* ma_l_info = *((info_table**)ma_l);
	int var_34 = 1;
	if(ma_l_info->type == 1)
	{
		if((((ma_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_35 = (Cons*)ma_l;
			ref ma_v_ref = (var_35)->C_element;
			put_binding(bindings, 36,  ma_v_ref);
			ref ma_n_ref = (var_35)->C_next;
			put_binding(bindings, 37,  ma_n_ref);
			ref ma_new_v_ref = create_thunk(bindings,  var_39);
			put_binding(bindings, 38,  ma_new_v_ref);
			ref ma_new_n_ref = create_thunk(bindings,  var_41);
			put_binding(bindings, 40,  ma_new_n_ref);
			NEW_REF(ma_new_res_ref, Cons*, sizeof(Cons), ma_new_res)
			(ma_new_res)->info_ptr = &Cons_info_table;
			(ma_new_res)->C_element = ma_new_v_ref;
			(ma_new_res)->C_next = ma_new_n_ref;
			put_binding(bindings, 42,  ma_new_res_ref);
			return ma_new_res_ref;
		}
		if((((ma_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_43 = (Nil*)ma_l;
			NEW_REF(ma_z_ref, Nil*, sizeof(Nil), ma_z)
			(ma_z)->info_ptr = &Nil_info_table;
			put_binding(bindings, 44,  ma_z_ref);
			return ma_z_ref;
		}
	}
	else
	{
		assert((ma_l_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"xorq %%rcx, %%rcx\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(ma_l_ref), "r"(var_3_cont), "r"(bindings), "r"(var_34),  "r"(ma_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_7(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref inc;
		get_binding(bindings, 5,  &(inc));
		ref list;
		get_binding(bindings, 8,  &(list));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(inc),  "r"(list)
			: "rdi",  "rsi"
		);
		goto *(void*)map;
	}
}
ref var_10(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref inced;
		get_binding(bindings, 6,  &(inced));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(inced)
			: "rdi"
		);
		goto *(void*)head;
	}
}
ref var_12(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref inced;
		get_binding(bindings, 6,  &(inced));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(inced)
			: "rdi"
		);
		goto *(void*)tail;
	}
}
ref var_14(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref tail1;
		get_binding(bindings, 11,  &(tail1));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(tail1)
			: "rdi"
		);
		goto *(void*)head;
	}
}
ref var_16(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref element1;
		get_binding(bindings, 9,  &(element1));
		ref element2;
		get_binding(bindings, 13,  &(element2));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(element1),  "r"(element2)
			: "rdi",  "rsi"
		);
		goto *(void*)plus_int;
	}
}
ref var_17_cont(hash_map* bindings)
{
	GET_BINDING(case_expr_ref, void**, case_expr, 15, bindings)
	info_table* case_expr_info = *((info_table**)case_expr);
	int var_45 = 15;
	if(case_expr_info->type == 1)
	{
		if((((case_expr_info)->extra).constructor).con_num == 0)
		{
			I* var_46 = (I*)case_expr;
			NEW_REF(x_ref, int*, sizeof(int), x)
			*(x) = (var_46)->I_value;
			put_binding(bindings, 47,  x_ref);
			GET_BINDING(var_49_ref, int*, var_49, 47, bindings)
			printf("%d\n",  *(var_49));
			NEW_REF(var_48, Unit*, sizeof(Unit), var_48_val)
			(var_48_val)->info_ptr = &Unit_info_table;
			return var_48;
		}
	}
	else
	{
		assert((case_expr_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"xorq %%rcx, %%rcx\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(case_expr_ref), "r"(var_17_cont), "r"(bindings), "r"(var_45),  "r"(case_expr_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_21_cont(hash_map* bindings)
{
	GET_BINDING(y1_ref, void**, y1, 1, bindings)
	info_table* y1_info = *((info_table**)y1);
	int var_50 = 1;
	if(y1_info->type == 1)
	{
		if((((y1_info)->extra).constructor).con_num == 0)
		{
			I* var_51 = (I*)y1;
			NEW_REF(b1_ref, int*, sizeof(int), b1)
			*(b1) = (var_51)->I_value;
			put_binding(bindings, 52,  b1_ref);
			NEW_REF(c1_ref, I*, sizeof(I), c1)
			(c1)->info_ptr = &I_info_table;
			int var_54;
			{
				GET_BINDING(a1_ref, int*, a1, 20, bindings)
				GET_BINDING(b1_ref, int*, b1, 52, bindings)
				var_54 = *(a1) + *(b1);
			}
			(c1)->I_value = var_54;
			put_binding(bindings, 53,  c1_ref);
			return c1_ref;
		}
	}
	else
	{
		assert((y1_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"xorq %%rcx, %%rcx\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(y1_ref), "r"(var_21_cont), "r"(bindings), "r"(var_50),  "r"(y1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_39(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	GET_BINDING(ma_f_ref, void**, ma_f, 0, bindings)
	int var_56 = 0;
	info_table ma_f_info = *(*((info_table**)ma_f));
	if(ma_f_info.type == 0)
	{
		if((((ma_f_info).layout).entries[0]).pointer)
		{
			ref ma_v_ref;
			get_binding(bindings, 36,  &(ma_v_ref));
			push_ptr(ma_v_ref);
		}
		else
		{
			GET_BINDING(ma_v_ref, int*, ma_v, 36, bindings)
			push_int(*(ma_v));
		}
		ref var_55;
		void* var_57 = (void*)(((ma_f_info).extra).function).slow_entry_point;
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			"jmp *%1;\n\t"
			: 
			: "r"(var_55),  "r"(var_57)
			: "rdi",  "rsi"
		);
	}
	else if(ma_f_info.type == 4)
		{
			if((((*((((ma_f_info).extra).pap_info).info_ptr)).layout).entries[0]).pointer)
			{
				ref ma_v_ref;
				get_binding(bindings, 36,  &(ma_v_ref));
				push_ptr(ma_v_ref);
			}
			else
			{
				GET_BINDING(ma_v_ref, int*, ma_v, 36, bindings)
				push_int(*(ma_v));
			}
			unroll_pap(ma_f);
			ref var_55;
			void* var_58 = (void*)(((*((((ma_f_info).extra).pap_info).info_ptr)).extra).function).slow_entry_point;
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %1, %%rsi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				"jmp *%1;\n\t"
				: 
				: "r"(var_55),  "r"(var_58)
				: "rdi",  "rsi"
			);
		}
	else
	{
		assert(ma_f_info.type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"xorq %%rcx, %%rcx\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(ma_f_ref), "r"(var_39), "r"(bindings), "r"(var_56),  "r"(ma_f_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_41(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref ma_f;
		get_binding(bindings, 0,  &(ma_f));
		ref ma_n;
		get_binding(bindings, 37,  &(ma_n));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(ma_f),  "r"(ma_n)
			: "rdi",  "rsi"
		);
		goto *(void*)map;
	}
}
ref plus_int(ref x1,  ref y1)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  x1);
	put_binding(bindings, 1,  y1);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_0_cont;
}
ref plus_int_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
	{
		ref x1;
		ref y1;
		pop_ptr(&(x1));
		pop_ptr(&(y1));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(x1),  "r"(y1)
			: "rdi",  "rsi"
		);
		goto *(void*)plus_int;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref x1;
			pop_ptr(&(x1));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(plus_int_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = x1;
			pap_[0] = pap__info;
		}
	else if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
		{
			ref x1;
			ref y1;
			pop_ptr(&(x1));
			pop_ptr(&(y1));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(plus_int_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref)+  sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = x1;
			*((ref*)((char*)pap_ + 0 + sizeof(ref) + sizeof(ref))) = y1;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_plus_int()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*2);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	layout_entries[1] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0 + sizeof(ref)};
	plus_int_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = plus_int_slow,  .arity = 2},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(plus_int_ref));
	*((info_table**)get_ref(plus_int_ref)) = &(plus_int_info_table);
}
ref head(ref he_l)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  he_l);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_1_cont;
}
ref head_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)))
	{
		ref he_l;
		pop_ptr(&(he_l));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(he_l)
			: "rdi"
		);
		goto *(void*)head;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref he_l;
			pop_ptr(&(he_l));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(head_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = he_l;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_head()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*1);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	head_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = head_slow,  .arity = 1},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(head_ref));
	*((info_table**)get_ref(head_ref)) = &(head_info_table);
}
ref tail(ref ta_l)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  ta_l);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_2_cont;
}
ref tail_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)))
	{
		ref ta_l;
		pop_ptr(&(ta_l));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(ta_l)
			: "rdi"
		);
		goto *(void*)tail;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref ta_l;
			pop_ptr(&(ta_l));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(tail_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = ta_l;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_tail()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*1);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	tail_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = tail_slow,  .arity = 1},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(tail_ref));
	*((info_table**)get_ref(tail_ref)) = &(tail_info_table);
}
ref map(ref ma_f,  ref ma_l)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  ma_f);
	put_binding(bindings, 1,  ma_l);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_3_cont;
}
ref map_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
	{
		ref ma_f;
		ref ma_l;
		pop_ptr(&(ma_f));
		pop_ptr(&(ma_l));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(ma_f),  "r"(ma_l)
			: "rdi",  "rsi"
		);
		goto *(void*)map;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref ma_f;
			pop_ptr(&(ma_f));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(map_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = ma_f;
			pap_[0] = pap__info;
		}
	else if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
		{
			ref ma_f;
			ref ma_l;
			pop_ptr(&(ma_f));
			pop_ptr(&(ma_l));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(map_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref)+  sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = ma_f;
			*((ref*)((char*)pap_ + 0 + sizeof(ref) + sizeof(ref))) = ma_l;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_map()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*2);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	layout_entries[1] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0 + sizeof(ref)};
	map_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = map_slow,  .arity = 2},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(map_ref));
	*((info_table**)get_ref(map_ref)) = &(map_info_table);
}
ref main_()
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	NEW_REF(one_ref, I*, sizeof(I), one)
	(one)->info_ptr = &I_info_table;
	(one)->I_value = 1;
	put_binding(bindings, 4,  one_ref);
	struct info_table* inc_info = (struct info_table*)new(sizeof(info_table));
	(inc_info)->type = 4;
	(inc_info)->extra.pap_info = (struct pap){.info_ptr = &(plus_int_info_table),  .size = 1};
	NEW_REF(inc_ref, void**, sizeof(ref) + sizeof(void*), inc)
	*((ref*)((char*)inc + 0 + sizeof(ref))) = one_ref;
	inc[0] = inc_info;
	put_binding(bindings, 5,  inc_ref);
	ref inced_ref = create_thunk(bindings,  var_7);
	put_binding(bindings, 6,  inced_ref);
	NEW_REF(list_ref, Cons*, sizeof(Cons), list)
	(list)->info_ptr = &Cons_info_table;
	(list)->C_element = one_ref;
	(list)->C_next = inced_ref;
	put_binding(bindings, 8,  list_ref);
	ref element1_ref = create_thunk(bindings,  var_10);
	put_binding(bindings, 9,  element1_ref);
	ref tail1_ref = create_thunk(bindings,  var_12);
	put_binding(bindings, 11,  tail1_ref);
	ref element2_ref = create_thunk(bindings,  var_14);
	put_binding(bindings, 13,  element2_ref);
	ref case_expr_ref = create_thunk(bindings,  var_16);
	put_binding(bindings, 15,  case_expr_ref);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_17_cont;
}
ref main_function()
{
	init_constructors_Int();
	init_constructors_List();
	init_constructors_Unit();
	init_function_plus_int();
	init_function_head();
	init_function_tail();
	init_function_map();
	ref var_59;
	var_59 = main_((ref){});
	while(stack_pointer < stack_top) {
		var_59 = case_continuation(updateContinuation(var_59));
	}
	return var_59;
}
