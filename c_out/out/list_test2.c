#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stg/plus_int/static.h"
#include "stg/bindings.h"
#include "containers/mmanager.h"
#include "stg/heap.h"
#include "data/string_.h"
ref var_61(ref thunk_ref);
ref var_59(ref thunk_ref);
ref var_42_cont(hash_map* bindings);
ref var_34(ref thunk_ref);
ref var_32(ref thunk_ref);
ref var_14_cont(hash_map* bindings);
ref var_9(ref thunk_ref);
ref var_5_cont(hash_map* bindings);
ref var_4_cont(hash_map* bindings);
ref var_3_cont(hash_map* bindings);
ref var_2_cont(hash_map* bindings);
ref var_1_cont(hash_map* bindings);
ref var_0_cont(hash_map* bindings);
ref main__slow(ref null);
ref main_();
void init_function_main_();
ref seq_slow(ref null);
ref seq(ref se_a,  ref se_b);
void init_function_seq();
ref print_i_list_slow(ref null);
ref print_i_list(ref pr_i_l);
void init_function_print_i_list();
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
info_table print_i_list_info_table;
ref print_i_list_ref;
info_table seq_info_table;
ref seq_ref;
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
	int var_11 = 0;
	if(x1_info->type == 1)
	{
		if((((x1_info)->extra).constructor).con_num == 0)
		{
			I* var_12 = (I*)x1;
			NEW_REF(a1_ref, int*, sizeof(int), a1)
			*(a1) = (var_12)->I_value;
			put_binding(bindings, 13,  a1_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_14_cont;
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
			: "r"(x1_ref), "r"(var_0_cont), "r"(bindings), "r"(var_11),  "r"(x1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_1_cont(hash_map* bindings)
{
	GET_BINDING(he_l_ref, void**, he_l, 0, bindings)
	info_table* he_l_info = *((info_table**)he_l);
	int var_15 = 0;
	if(he_l_info->type == 1)
	{
		if((((he_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_16 = (Cons*)he_l;
			ref he_v_ref = (var_16)->C_element;
			put_binding(bindings, 17,  he_v_ref);
			ref he_n_ref = (var_16)->C_next;
			put_binding(bindings, 18,  he_n_ref);
			return he_v_ref;
		}
		if((((he_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_19 = (Nil*)he_l;
			assert(false);
			ref var_20;
			return var_20;
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
			: "r"(he_l_ref), "r"(var_1_cont), "r"(bindings), "r"(var_15),  "r"(he_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_2_cont(hash_map* bindings)
{
	GET_BINDING(ta_l_ref, void**, ta_l, 0, bindings)
	info_table* ta_l_info = *((info_table**)ta_l);
	int var_21 = 0;
	if(ta_l_info->type == 1)
	{
		if((((ta_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_22 = (Cons*)ta_l;
			ref ta_v_ref = (var_22)->C_element;
			put_binding(bindings, 23,  ta_v_ref);
			ref ta_n_ref = (var_22)->C_next;
			put_binding(bindings, 24,  ta_n_ref);
			return ta_n_ref;
		}
		if((((ta_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_25 = (Nil*)ta_l;
			assert(false);
			ref var_26;
			return var_26;
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
			: "r"(ta_l_ref), "r"(var_2_cont), "r"(bindings), "r"(var_21),  "r"(ta_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_3_cont(hash_map* bindings)
{
	GET_BINDING(ma_l_ref, void**, ma_l, 1, bindings)
	info_table* ma_l_info = *((info_table**)ma_l);
	int var_27 = 1;
	if(ma_l_info->type == 1)
	{
		if((((ma_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_28 = (Cons*)ma_l;
			ref ma_v_ref = (var_28)->C_element;
			put_binding(bindings, 29,  ma_v_ref);
			ref ma_n_ref = (var_28)->C_next;
			put_binding(bindings, 30,  ma_n_ref);
			ref ma_new_v_ref = create_thunk(bindings,  var_32);
			put_binding(bindings, 31,  ma_new_v_ref);
			ref ma_new_n_ref = create_thunk(bindings,  var_34);
			put_binding(bindings, 33,  ma_new_n_ref);
			NEW_REF(ma_new_res_ref, Cons*, sizeof(Cons), ma_new_res)
			(ma_new_res)->info_ptr = &Cons_info_table;
			(ma_new_res)->C_element = ma_new_v_ref;
			(ma_new_res)->C_next = ma_new_n_ref;
			put_binding(bindings, 35,  ma_new_res_ref);
			return ma_new_res_ref;
		}
		if((((ma_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_36 = (Nil*)ma_l;
			NEW_REF(ma_z_ref, Nil*, sizeof(Nil), ma_z)
			(ma_z)->info_ptr = &Nil_info_table;
			put_binding(bindings, 37,  ma_z_ref);
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
			: "r"(ma_l_ref), "r"(var_3_cont), "r"(bindings), "r"(var_27),  "r"(ma_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_4_cont(hash_map* bindings)
{
	GET_BINDING(pr_i_l_ref, void**, pr_i_l, 0, bindings)
	info_table* pr_i_l_info = *((info_table**)pr_i_l);
	int var_38 = 0;
	if(pr_i_l_info->type == 1)
	{
		if((((pr_i_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_39 = (Cons*)pr_i_l;
			ref pr_i_v_ref = (var_39)->C_element;
			put_binding(bindings, 40,  pr_i_v_ref);
			ref pr_i_n_ref = (var_39)->C_next;
			put_binding(bindings, 41,  pr_i_n_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_42_cont;
		}
		if((((pr_i_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_43 = (Nil*)pr_i_l;
			NEW_REF(unit_ref, Unit*, sizeof(Unit), unit)
			(unit)->info_ptr = &Unit_info_table;
			put_binding(bindings, 44,  unit_ref);
			return unit_ref;
		}
	}
	else
	{
		assert((pr_i_l_info)->type == 5);
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
			: "r"(pr_i_l_ref), "r"(var_4_cont), "r"(bindings), "r"(var_38),  "r"(pr_i_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_5_cont(hash_map* bindings)
{
	GET_BINDING(se_a_ref, void**, se_a, 0, bindings)
	info_table* se_a_info = *((info_table**)se_a);
	int var_45 = 0;
	if(se_a_info->type == 5)
	{
		assert((se_a_info)->type == 5);
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
			: "r"(se_a_ref), "r"(var_5_cont), "r"(bindings), "r"(var_45),  "r"(se_a_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
	else
	{
		ref se_b_ref;
		get_binding(bindings, 1,  &(se_b_ref));
		return se_b_ref;
	}
}
ref var_9(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref inc;
		get_binding(bindings, 7,  &(inc));
		ref list;
		get_binding(bindings, 10,  &(list));
		return map(inc,  list);
	}
}
ref var_14_cont(hash_map* bindings)
{
	GET_BINDING(y1_ref, void**, y1, 1, bindings)
	info_table* y1_info = *((info_table**)y1);
	int var_46 = 1;
	if(y1_info->type == 1)
	{
		if((((y1_info)->extra).constructor).con_num == 0)
		{
			I* var_47 = (I*)y1;
			NEW_REF(b1_ref, int*, sizeof(int), b1)
			*(b1) = (var_47)->I_value;
			put_binding(bindings, 48,  b1_ref);
			NEW_REF(c1_ref, I*, sizeof(I), c1)
			(c1)->info_ptr = &I_info_table;
			int var_50;
			{
				GET_BINDING(a1_ref, int*, a1, 13, bindings)
				GET_BINDING(b1_ref, int*, b1, 48, bindings)
				var_50 = *(a1) + *(b1);
			}
			(c1)->I_value = var_50;
			put_binding(bindings, 49,  c1_ref);
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
			: "r"(y1_ref), "r"(var_14_cont), "r"(bindings), "r"(var_46),  "r"(y1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_32(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	GET_BINDING(ma_f_ref, void**, ma_f, 0, bindings)
	int var_52 = 0;
	info_table ma_f_info = *(*((info_table**)ma_f));
	if(ma_f_info.type == 0)
	{
		if((((ma_f_info).layout).entries[0]).pointer)
		{
			ref ma_v_ref;
			get_binding(bindings, 29,  &(ma_v_ref));
			push_ptr(ma_v_ref);
		}
		else
		{
			GET_BINDING(ma_v_ref, int*, ma_v, 29, bindings)
			push_int(*(ma_v));
		}
		ref var_51;
		void* var_53 = (void*)(((ma_f_info).extra).function).slow_entry_point;
		__asm__ volatile   goto (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			"jmp *%1;\n\t"
			: 
			: "r"(var_51),  "r"(var_53)
			: "rdi",  "rsi"
		);
	}
	else if(ma_f_info.type == 4)
		{
			if((((*((((ma_f_info).extra).pap_info).info_ptr)).layout).entries[0]).pointer)
			{
				ref ma_v_ref;
				get_binding(bindings, 29,  &(ma_v_ref));
				push_ptr(ma_v_ref);
			}
			else
			{
				GET_BINDING(ma_v_ref, int*, ma_v, 29, bindings)
				push_int(*(ma_v));
			}
			unroll_pap(ma_f);
			ref var_51;
			void* var_53 = (void*)(((*((((ma_f_info).extra).pap_info).info_ptr)).extra).function).slow_entry_point;
			__asm__ volatile   goto (
				"movq %0, %%rdi;\n\t"
				"movq %1, %%rsi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				"jmp *%1;\n\t"
				: 
				: "r"(var_51),  "r"(var_54)
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
			: "r"(ma_f_ref), "r"(var_32), "r"(bindings), "r"(var_52),  "r"(ma_f_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_34(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref ma_f;
		get_binding(bindings, 0,  &(ma_f));
		ref ma_n;
		get_binding(bindings, 30,  &(ma_n));
		return map(ma_f,  ma_n);
	}
}
ref var_42_cont(hash_map* bindings)
{
	GET_BINDING(pr_i_v_ref, void**, pr_i_v, 40, bindings)
	info_table* pr_i_v_info = *((info_table**)pr_i_v);
	int var_55 = 40;
	if(pr_i_v_info->type == 1)
	{
		if((((pr_i_v_info)->extra).constructor).con_num == 0)
		{
			I* var_56 = (I*)pr_i_v;
			NEW_REF(pr_i_x_ref, int*, sizeof(int), pr_i_x)
			*(pr_i_x) = (var_56)->I_value;
			put_binding(bindings, 57,  pr_i_x_ref);
			ref pr_i_rest_ref = create_thunk(bindings,  var_59);
			put_binding(bindings, 58,  pr_i_rest_ref);
			ref pr_i_inner_ref = create_thunk(bindings,  var_61);
			put_binding(bindings, 60,  pr_i_inner_ref);
			NEW_REF(unit_ref, Unit*, sizeof(Unit), unit)
			(unit)->info_ptr = &Unit_info_table;
			put_binding(bindings, 62,  unit_ref);
			{
				ref pr_i_inner;
				get_binding(bindings, 60,  &(pr_i_inner));
				ref unit;
				get_binding(bindings, 62,  &(unit));
				return seq(pr_i_inner,  unit);
			}
		}
	}
	else
	{
		assert((pr_i_v_info)->type == 5);
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
			: "r"(pr_i_v_ref), "r"(var_42_cont), "r"(bindings), "r"(var_55),  "r"(pr_i_v_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_59(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref pr_i_n;
		get_binding(bindings, 41,  &(pr_i_n));
		return print_i_list(pr_i_n);
	}
}
ref var_61(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		GET_BINDING(var_64_ref, int*, var_64, 57, bindings)
		printf("%d\n",  *(var_64));
		NEW_REF(var_63, Unit*, sizeof(Unit), var_63_val)
		(var_63_val)->info_ptr = &Unit_info_table;
		ref pr_i_rest;
		get_binding(bindings, 58,  &(pr_i_rest));
		return seq(var_63,  pr_i_rest);
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
ref print_i_list(ref pr_i_l)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  pr_i_l);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_4_cont;
}
ref print_i_list_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)))
	{
		ref pr_i_l;
		pop_ptr(&(pr_i_l));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(pr_i_l)
			: "rdi"
		);
		goto *(void*)print_i_list;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref pr_i_l;
			pop_ptr(&(pr_i_l));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(print_i_list_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = pr_i_l;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_print_i_list()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*1);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	print_i_list_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = print_i_list_slow,  .arity = 1},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(print_i_list_ref));
	*((info_table**)get_ref(print_i_list_ref)) = &(print_i_list_info_table);
}
ref seq(ref se_a,  ref se_b)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  se_a);
	put_binding(bindings, 1,  se_b);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_5_cont;
}
ref seq_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
	{
		ref se_a;
		ref se_b;
		pop_ptr(&(se_a));
		pop_ptr(&(se_b));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(se_a),  "r"(se_b)
			: "rdi",  "rsi"
		);
		goto *(void*)seq;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref se_a;
			pop_ptr(&(se_a));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(seq_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = se_a;
			pap_[0] = pap__info;
		}
	else if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
		{
			ref se_a;
			ref se_b;
			pop_ptr(&(se_a));
			pop_ptr(&(se_b));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(seq_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref)+  sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = se_a;
			*((ref*)((char*)pap_ + 0 + sizeof(ref) + sizeof(ref))) = se_b;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_seq()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*2);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	layout_entries[1] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0 + sizeof(ref)};
	seq_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = seq_slow,  .arity = 2},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(seq_ref));
	*((info_table**)get_ref(seq_ref)) = &(seq_info_table);
}
ref main_()
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	NEW_REF(one_ref, I*, sizeof(I), one)
	(one)->info_ptr = &I_info_table;
	(one)->I_value = 1;
	put_binding(bindings, 6,  one_ref);
	struct info_table* inc_info = (struct info_table*)new(sizeof(info_table));
	(inc_info)->type = 4;
	(inc_info)->extra.pap_info = (struct pap){.info_ptr = &(plus_int_info_table),  .size = 1};
	NEW_REF(inc_ref, void**, sizeof(ref) + sizeof(void*), inc)
	*((ref*)((char*)inc + 0 + sizeof(ref))) = one_ref;
	inc[0] = inc_info;
	put_binding(bindings, 7,  inc_ref);
	ref inced_ref = create_thunk(bindings,  var_9);
	put_binding(bindings, 8,  inced_ref);
	NEW_REF(list_ref, Cons*, sizeof(Cons), list)
	(list)->info_ptr = &Cons_info_table;
	(list)->C_element = one_ref;
	(list)->C_next = inced_ref;
	put_binding(bindings, 10,  list_ref);
	{
		ref list;
		get_binding(bindings, 10,  &(list));
		return print_i_list(list);
	}
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
	init_function_print_i_list();
	init_function_seq();
	main_((ref){});
}
