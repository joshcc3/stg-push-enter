#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stg/plus_int/static.h"
#include "stg/bindings.h"
#include "containers/mmanager.h"
#include "stg/heap.h"
#include "data/string_.h"
ref var_94(ref thunk_ref);
ref var_92(ref thunk_ref);
ref var_86(ref thunk_ref);
ref var_65_cont(hash_map* bindings);
ref var_52_cont(hash_map* bindings);
ref var_42(ref thunk_ref);
ref var_40(ref thunk_ref);
ref var_22_cont(hash_map* bindings);
ref var_16(ref thunk_ref);
ref var_13(ref thunk_ref);
ref var_11(ref thunk_ref);
ref var_7_cont(hash_map* bindings);
ref var_6_cont(hash_map* bindings);
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
ref uncurry_slow(ref null);
ref uncurry(ref un_f,  ref un_p);
void init_function_uncurry();
ref zip_slow(ref null);
ref zip(ref zi_as,  ref zi_bs);
void init_function_zip();
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
void init_constructors_Pair();
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
info_table zip_info_table;
ref zip_ref;
info_table uncurry_info_table;
ref uncurry_ref;
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
struct info_table Pa_info_table;
typedef struct Pa {
	info_table* info_ptr;
	ref P_fst;
	ref P_snd;
} Pa;
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
void init_constructors_Pair()
{
	Pa_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 2, .con_num = 0,  .con_name = (string_){.char_arr = "Pa",  .length = 2}}}};
}
ref var_0_cont(hash_map* bindings)
{
	GET_BINDING(x1_ref, void**, x1, 0, bindings)
	info_table* x1_info = *((info_table**)x1);
	int var_19 = 0;
	if(x1_info->type == 1)
	{
		if((((x1_info)->extra).constructor).con_num == 0)
		{
			I* var_20 = (I*)x1;
			NEW_REF(a1_ref, int*, sizeof(int), a1)
			*(a1) = (var_20)->I_value;
			put_binding(bindings, 21,  a1_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_22_cont;
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
			: "r"(x1_ref), "r"(var_0_cont), "r"(bindings), "r"(var_19),  "r"(x1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_1_cont(hash_map* bindings)
{
	GET_BINDING(he_l_ref, void**, he_l, 0, bindings)
	info_table* he_l_info = *((info_table**)he_l);
	int var_23 = 0;
	if(he_l_info->type == 1)
	{
		if((((he_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_24 = (Cons*)he_l;
			ref he_v_ref = (var_24)->C_element;
			put_binding(bindings, 25,  he_v_ref);
			ref he_n_ref = (var_24)->C_next;
			put_binding(bindings, 26,  he_n_ref);
			return he_v_ref;
		}
		if((((he_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_27 = (Nil*)he_l;
			assert(false);
			ref var_28;
			return var_28;
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
			: "r"(he_l_ref), "r"(var_1_cont), "r"(bindings), "r"(var_23),  "r"(he_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_2_cont(hash_map* bindings)
{
	GET_BINDING(ta_l_ref, void**, ta_l, 0, bindings)
	info_table* ta_l_info = *((info_table**)ta_l);
	int var_29 = 0;
	if(ta_l_info->type == 1)
	{
		if((((ta_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_30 = (Cons*)ta_l;
			ref ta_v_ref = (var_30)->C_element;
			put_binding(bindings, 31,  ta_v_ref);
			ref ta_n_ref = (var_30)->C_next;
			put_binding(bindings, 32,  ta_n_ref);
			return ta_n_ref;
		}
		if((((ta_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_33 = (Nil*)ta_l;
			assert(false);
			ref var_34;
			return var_34;
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
			: "r"(ta_l_ref), "r"(var_2_cont), "r"(bindings), "r"(var_29),  "r"(ta_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_3_cont(hash_map* bindings)
{
	GET_BINDING(ma_l_ref, void**, ma_l, 1, bindings)
	info_table* ma_l_info = *((info_table**)ma_l);
	int var_35 = 1;
	if(ma_l_info->type == 1)
	{
		if((((ma_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_36 = (Cons*)ma_l;
			ref ma_v_ref = (var_36)->C_element;
			put_binding(bindings, 37,  ma_v_ref);
			ref ma_n_ref = (var_36)->C_next;
			put_binding(bindings, 38,  ma_n_ref);
			ref ma_new_v_ref = create_thunk(bindings,  var_40);
			put_binding(bindings, 39,  ma_new_v_ref);
			ref ma_new_n_ref = create_thunk(bindings,  var_42);
			put_binding(bindings, 41,  ma_new_n_ref);
			NEW_REF(ma_new_res_ref, Cons*, sizeof(Cons), ma_new_res)
			(ma_new_res)->info_ptr = &Cons_info_table;
			(ma_new_res)->C_element = ma_new_v_ref;
			(ma_new_res)->C_next = ma_new_n_ref;
			put_binding(bindings, 43,  ma_new_res_ref);
			return ma_new_res_ref;
		}
		if((((ma_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_44 = (Nil*)ma_l;
			NEW_REF(ma_z_ref, Nil*, sizeof(Nil), ma_z)
			(ma_z)->info_ptr = &Nil_info_table;
			put_binding(bindings, 45,  ma_z_ref);
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
			: "r"(ma_l_ref), "r"(var_3_cont), "r"(bindings), "r"(var_35),  "r"(ma_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_4_cont(hash_map* bindings)
{
	GET_BINDING(zi_as_ref, void**, zi_as, 0, bindings)
	info_table* zi_as_info = *((info_table**)zi_as);
	int var_46 = 0;
	if(zi_as_info->type == 1)
	{
		if((((zi_as_info)->extra).constructor).con_num == 1)
		{
			Nil* var_47 = (Nil*)zi_as;
			NEW_REF(zi_nil_ref, Nil*, sizeof(Nil), zi_nil)
			(zi_nil)->info_ptr = &Nil_info_table;
			put_binding(bindings, 48,  zi_nil_ref);
			return zi_nil_ref;
		}
		if((((zi_as_info)->extra).constructor).con_num == 0)
		{
			Cons* var_49 = (Cons*)zi_as;
			ref zi_a_ref = (var_49)->C_element;
			put_binding(bindings, 50,  zi_a_ref);
			ref zi_an_ref = (var_49)->C_next;
			put_binding(bindings, 51,  zi_an_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_52_cont;
		}
	}
	else
	{
		assert((zi_as_info)->type == 5);
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
			: "r"(zi_as_ref), "r"(var_4_cont), "r"(bindings), "r"(var_46),  "r"(zi_as_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_5_cont(hash_map* bindings)
{
	GET_BINDING(un_p_ref, void**, un_p, 1, bindings)
	info_table* un_p_info = *((info_table**)un_p);
	int var_53 = 1;
	if(un_p_info->type == 1)
	{
		if((((un_p_info)->extra).constructor).con_num == 0)
		{
			Pa* var_54 = (Pa*)un_p;
			ref un_a_ref = (var_54)->P_fst;
			put_binding(bindings, 55,  un_a_ref);
			ref un_b_ref = (var_54)->P_snd;
			put_binding(bindings, 56,  un_b_ref);
			GET_BINDING(un_f_ref, void**, un_f, 0, bindings)
			int var_58 = 0;
			info_table un_f_info = *(*((info_table**)un_f));
			if(un_f_info.type == 0)
			{
				if((((un_f_info).layout).entries[1]).pointer)
				{
					ref un_b_ref;
					get_binding(bindings, 56,  &(un_b_ref));
					push_ptr(un_b_ref);
				}
				else
				{
					GET_BINDING(un_b_ref, int*, un_b, 56, bindings)
					push_int(*(un_b));
				}
				if((((un_f_info).layout).entries[0]).pointer)
				{
					ref un_a_ref;
					get_binding(bindings, 55,  &(un_a_ref));
					push_ptr(un_a_ref);
				}
				else
				{
					GET_BINDING(un_a_ref, int*, un_a, 55, bindings)
					push_int(*(un_a));
				}
				ref var_57;
				void* var_59 = (void*)(((un_f_info).extra).function).slow_entry_point;
				__asm__ volatile (
					"movq %0, %%rdi;\n\t"
					"movq %1, %%rsi;\n\t"
					"movq %%rbp, %%rsp;\n\t"
					"popq %%rbp;\n\t"
					"jmp *%1;\n\t"
					: 
					: "r"(var_57),  "r"(var_59)
					: "rdi",  "rsi"
				);
			}
			else if(un_f_info.type == 4)
				{
					if((((*((((un_f_info).extra).pap_info).info_ptr)).layout).entries[1]).pointer)
					{
						ref un_b_ref;
						get_binding(bindings, 56,  &(un_b_ref));
						push_ptr(un_b_ref);
					}
					else
					{
						GET_BINDING(un_b_ref, int*, un_b, 56, bindings)
						push_int(*(un_b));
					}
					if((((*((((un_f_info).extra).pap_info).info_ptr)).layout).entries[0]).pointer)
					{
						ref un_a_ref;
						get_binding(bindings, 55,  &(un_a_ref));
						push_ptr(un_a_ref);
					}
					else
					{
						GET_BINDING(un_a_ref, int*, un_a, 55, bindings)
						push_int(*(un_a));
					}
					unroll_pap(un_f);
					ref var_57;
					void* var_60 = (void*)(((*((((un_f_info).extra).pap_info).info_ptr)).extra).function).slow_entry_point;
					__asm__ volatile (
						"movq %0, %%rdi;\n\t"
						"movq %1, %%rsi;\n\t"
						"movq %%rbp, %%rsp;\n\t"
						"popq %%rbp;\n\t"
						"jmp *%1;\n\t"
						: 
						: "r"(var_57),  "r"(var_60)
						: "rdi",  "rsi"
					);
				}
			else
			{
				assert(un_f_info.type == 5);
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
					: "r"(un_f_ref), "r"(var_5_cont), "r"(bindings), "r"(var_58),  "r"(un_f_ref)
					: "rdi", "rsi", "rdx", "rcx",  "r8"
				);
				goto *(void*)thunk_continuation;
			}
		}
	}
	else
	{
		assert((un_p_info)->type == 5);
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
			: "r"(un_p_ref), "r"(var_5_cont), "r"(bindings), "r"(var_53),  "r"(un_p_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_6_cont(hash_map* bindings)
{
	GET_BINDING(pr_i_l_ref, void**, pr_i_l, 0, bindings)
	info_table* pr_i_l_info = *((info_table**)pr_i_l);
	int var_61 = 0;
	if(pr_i_l_info->type == 1)
	{
		if((((pr_i_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_62 = (Cons*)pr_i_l;
			ref pr_i_v_ref = (var_62)->C_element;
			put_binding(bindings, 63,  pr_i_v_ref);
			ref pr_i_n_ref = (var_62)->C_next;
			put_binding(bindings, 64,  pr_i_n_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_65_cont;
		}
		if((((pr_i_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_66 = (Nil*)pr_i_l;
			NEW_REF(unit_ref, Unit*, sizeof(Unit), unit)
			(unit)->info_ptr = &Unit_info_table;
			put_binding(bindings, 67,  unit_ref);
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
			: "r"(pr_i_l_ref), "r"(var_6_cont), "r"(bindings), "r"(var_61),  "r"(pr_i_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_7_cont(hash_map* bindings)
{
	GET_BINDING(se_a_ref, void**, se_a, 0, bindings)
	info_table* se_a_info = *((info_table**)se_a);
	int var_68 = 0;
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
			: "r"(se_a_ref), "r"(var_7_cont), "r"(bindings), "r"(var_68),  "r"(se_a_ref)
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
ref var_11(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref fibs;
		get_binding(bindings, 18,  &(fibs));
		ref fibTail;
		get_binding(bindings, 12,  &(fibTail));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(fibs),  "r"(fibTail)
			: "rdi",  "rsi"
		);
		goto *(void*)zip;
	}
}
ref var_13(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref fibs;
		get_binding(bindings, 18,  &(fibs));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(fibs)
			: "rdi"
		);
		goto *(void*)tail;
	}
}
ref var_16(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref plus_uncurried;
		get_binding(bindings, 14,  &(plus_uncurried));
		ref zipped;
		get_binding(bindings, 10,  &(zipped));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(plus_uncurried),  "r"(zipped)
			: "rdi",  "rsi"
		);
		goto *(void*)map;
	}
}
ref var_22_cont(hash_map* bindings)
{
	GET_BINDING(y1_ref, void**, y1, 1, bindings)
	info_table* y1_info = *((info_table**)y1);
	int var_69 = 1;
	if(y1_info->type == 1)
	{
		if((((y1_info)->extra).constructor).con_num == 0)
		{
			I* var_70 = (I*)y1;
			NEW_REF(b1_ref, int*, sizeof(int), b1)
			*(b1) = (var_70)->I_value;
			put_binding(bindings, 71,  b1_ref);
			NEW_REF(c1_ref, I*, sizeof(I), c1)
			(c1)->info_ptr = &I_info_table;
			int var_73;
			{
				GET_BINDING(a1_ref, int*, a1, 21, bindings)
				GET_BINDING(b1_ref, int*, b1, 71, bindings)
				var_73 = *(a1) + *(b1);
			}
			(c1)->I_value = var_73;
			put_binding(bindings, 72,  c1_ref);
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
			: "r"(y1_ref), "r"(var_22_cont), "r"(bindings), "r"(var_69),  "r"(y1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_40(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	GET_BINDING(ma_f_ref, void**, ma_f, 0, bindings)
	int var_75 = 0;
	info_table ma_f_info = *(*((info_table**)ma_f));
	if(ma_f_info.type == 0)
	{
		if((((ma_f_info).layout).entries[0]).pointer)
		{
			ref ma_v_ref;
			get_binding(bindings, 37,  &(ma_v_ref));
			push_ptr(ma_v_ref);
		}
		else
		{
			GET_BINDING(ma_v_ref, int*, ma_v, 37, bindings)
			push_int(*(ma_v));
		}
		ref var_74;
		void* var_76 = (void*)(((ma_f_info).extra).function).slow_entry_point;
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			"jmp *%1;\n\t"
			: 
			: "r"(var_74),  "r"(var_76)
			: "rdi",  "rsi"
		);
	}
	else if(ma_f_info.type == 4)
		{
			if((((*((((ma_f_info).extra).pap_info).info_ptr)).layout).entries[0]).pointer)
			{
				ref ma_v_ref;
				get_binding(bindings, 37,  &(ma_v_ref));
				push_ptr(ma_v_ref);
			}
			else
			{
				GET_BINDING(ma_v_ref, int*, ma_v, 37, bindings)
				push_int(*(ma_v));
			}
			unroll_pap(ma_f);
			ref var_74;
			void* var_77 = (void*)(((*((((ma_f_info).extra).pap_info).info_ptr)).extra).function).slow_entry_point;
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %1, %%rsi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				"jmp *%1;\n\t"
				: 
				: "r"(var_74),  "r"(var_77)
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
			: "r"(ma_f_ref), "r"(var_40), "r"(bindings), "r"(var_75),  "r"(ma_f_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_42(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref ma_f;
		get_binding(bindings, 0,  &(ma_f));
		ref ma_n;
		get_binding(bindings, 38,  &(ma_n));
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
ref var_52_cont(hash_map* bindings)
{
	GET_BINDING(zi_bs_ref, void**, zi_bs, 1, bindings)
	info_table* zi_bs_info = *((info_table**)zi_bs);
	int var_78 = 1;
	if(zi_bs_info->type == 1)
	{
		if((((zi_bs_info)->extra).constructor).con_num == 1)
		{
			Nil* var_79 = (Nil*)zi_bs;
			NEW_REF(zi_nil_ref, Nil*, sizeof(Nil), zi_nil)
			(zi_nil)->info_ptr = &Nil_info_table;
			put_binding(bindings, 80,  zi_nil_ref);
			return zi_nil_ref;
		}
		if((((zi_bs_info)->extra).constructor).con_num == 0)
		{
			Cons* var_81 = (Cons*)zi_bs;
			ref zi_b_ref = (var_81)->C_element;
			put_binding(bindings, 82,  zi_b_ref);
			ref zi_bn_ref = (var_81)->C_next;
			put_binding(bindings, 83,  zi_bn_ref);
			NEW_REF(zi_p_ref, Pa*, sizeof(Pa), zi_p)
			(zi_p)->info_ptr = &Pa_info_table;
			GET_BINDING(zi_a_ref, void**, zi_a, 50, bindings)
			(zi_p)->P_fst = zi_a_ref;
			(zi_p)->P_snd = zi_b_ref;
			put_binding(bindings, 84,  zi_p_ref);
			ref zi_rest_ref = create_thunk(bindings,  var_86);
			put_binding(bindings, 85,  zi_rest_ref);
			NEW_REF(zi_res_ref, Cons*, sizeof(Cons), zi_res)
			(zi_res)->info_ptr = &Cons_info_table;
			(zi_res)->C_element = zi_p_ref;
			(zi_res)->C_next = zi_rest_ref;
			put_binding(bindings, 87,  zi_res_ref);
			return zi_res_ref;
		}
	}
	else
	{
		assert((zi_bs_info)->type == 5);
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
			: "r"(zi_bs_ref), "r"(var_52_cont), "r"(bindings), "r"(var_78),  "r"(zi_bs_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_65_cont(hash_map* bindings)
{
	GET_BINDING(pr_i_v_ref, void**, pr_i_v, 63, bindings)
	info_table* pr_i_v_info = *((info_table**)pr_i_v);
	int var_88 = 63;
	if(pr_i_v_info->type == 1)
	{
		if((((pr_i_v_info)->extra).constructor).con_num == 0)
		{
			I* var_89 = (I*)pr_i_v;
			NEW_REF(pr_i_x_ref, int*, sizeof(int), pr_i_x)
			*(pr_i_x) = (var_89)->I_value;
			put_binding(bindings, 90,  pr_i_x_ref);
			ref pr_i_rest_ref = create_thunk(bindings,  var_92);
			put_binding(bindings, 91,  pr_i_rest_ref);
			ref pr_i_inner_ref = create_thunk(bindings,  var_94);
			put_binding(bindings, 93,  pr_i_inner_ref);
			NEW_REF(unit_ref, Unit*, sizeof(Unit), unit)
			(unit)->info_ptr = &Unit_info_table;
			put_binding(bindings, 95,  unit_ref);
			{
				ref pr_i_inner;
				get_binding(bindings, 93,  &(pr_i_inner));
				ref unit;
				get_binding(bindings, 95,  &(unit));
				__asm__ volatile (
					"movq %0, %%rdi;\n\t"
					"movq %1, %%rsi;\n\t"
					"movq %%rbp, %%rsp;\n\t"
					"popq %%rbp;\n\t"
					: 
					: "r"(pr_i_inner),  "r"(unit)
					: "rdi",  "rsi"
				);
				goto *(void*)seq;
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
			: "r"(pr_i_v_ref), "r"(var_65_cont), "r"(bindings), "r"(var_88),  "r"(pr_i_v_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_86(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref zi_an;
		get_binding(bindings, 51,  &(zi_an));
		ref zi_bn;
		get_binding(bindings, 83,  &(zi_bn));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(zi_an),  "r"(zi_bn)
			: "rdi",  "rsi"
		);
		goto *(void*)zip;
	}
}
ref var_92(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref pr_i_n;
		get_binding(bindings, 64,  &(pr_i_n));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(pr_i_n)
			: "rdi"
		);
		goto *(void*)print_i_list;
	}
}
ref var_94(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		GET_BINDING(var_97_ref, int*, var_97, 90, bindings)
		printf("%d\n",  *(var_97));
		NEW_REF(var_96, Unit*, sizeof(Unit), var_96_val)
		(var_96_val)->info_ptr = &Unit_info_table;
		ref pr_i_rest;
		get_binding(bindings, 91,  &(pr_i_rest));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(var_96),  "r"(pr_i_rest)
			: "rdi",  "rsi"
		);
		goto *(void*)seq;
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
ref zip(ref zi_as,  ref zi_bs)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  zi_as);
	put_binding(bindings, 1,  zi_bs);
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
ref zip_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
	{
		ref zi_as;
		ref zi_bs;
		pop_ptr(&(zi_as));
		pop_ptr(&(zi_bs));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(zi_as),  "r"(zi_bs)
			: "rdi",  "rsi"
		);
		goto *(void*)zip;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref zi_as;
			pop_ptr(&(zi_as));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(zip_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = zi_as;
			pap_[0] = pap__info;
		}
	else if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
		{
			ref zi_as;
			ref zi_bs;
			pop_ptr(&(zi_as));
			pop_ptr(&(zi_bs));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(zip_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref)+  sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = zi_as;
			*((ref*)((char*)pap_ + 0 + sizeof(ref) + sizeof(ref))) = zi_bs;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_zip()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*2);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	layout_entries[1] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0 + sizeof(ref)};
	zip_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = zip_slow,  .arity = 2},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(zip_ref));
	*((info_table**)get_ref(zip_ref)) = &(zip_info_table);
}
ref uncurry(ref un_f,  ref un_p)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  un_f);
	put_binding(bindings, 1,  un_p);
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
ref uncurry_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
	{
		ref un_f;
		ref un_p;
		pop_ptr(&(un_f));
		pop_ptr(&(un_p));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(un_f),  "r"(un_p)
			: "rdi",  "rsi"
		);
		goto *(void*)uncurry;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref un_f;
			pop_ptr(&(un_f));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(uncurry_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = un_f;
			pap_[0] = pap__info;
		}
	else if(arg_satisfaction_check(sizeof(ref)+  sizeof(ref)))
		{
			ref un_f;
			ref un_p;
			pop_ptr(&(un_f));
			pop_ptr(&(un_p));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(uncurry_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref)+  sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = un_f;
			*((ref*)((char*)pap_ + 0 + sizeof(ref) + sizeof(ref))) = un_p;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_uncurry()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*2);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	layout_entries[1] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0 + sizeof(ref)};
	uncurry_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = uncurry_slow,  .arity = 2},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(uncurry_ref));
	*((info_table**)get_ref(uncurry_ref)) = &(uncurry_info_table);
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
	goto *(void*)var_6_cont;
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
	goto *(void*)var_7_cont;
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
	put_binding(bindings, 8,  one_ref);
	NEW_REF(zero_ref, I*, sizeof(I), zero)
	(zero)->info_ptr = &I_info_table;
	(zero)->I_value = 0;
	put_binding(bindings, 9,  zero_ref);
	ref zipped_ref = create_thunk(bindings,  var_11);
	put_binding(bindings, 10,  zipped_ref);
	ref fibTail_ref = create_thunk(bindings,  var_13);
	put_binding(bindings, 12,  fibTail_ref);
	struct info_table* plus_uncurried_info = (struct info_table*)new(sizeof(info_table));
	(plus_uncurried_info)->type = 4;
	(plus_uncurried_info)->extra.pap_info = (struct pap){.info_ptr = &(uncurry_info_table),  .size = 1};
	NEW_REF(plus_uncurried_ref, void**, sizeof(ref) + sizeof(void*), plus_uncurried)
	*((ref*)((char*)plus_uncurried + 0 + sizeof(ref))) = plus_int_ref;
	plus_uncurried[0] = plus_uncurried_info;
	put_binding(bindings, 14,  plus_uncurried_ref);
	ref mapped_ref = create_thunk(bindings,  var_16);
	put_binding(bindings, 15,  mapped_ref);
	NEW_REF(fibsT_ref, Cons*, sizeof(Cons), fibsT)
	(fibsT)->info_ptr = &Cons_info_table;
	(fibsT)->C_element = one_ref;
	(fibsT)->C_next = mapped_ref;
	put_binding(bindings, 17,  fibsT_ref);
	NEW_REF(fibs_ref, Cons*, sizeof(Cons), fibs)
	(fibs)->info_ptr = &Cons_info_table;
	(fibs)->C_element = zero_ref;
	(fibs)->C_next = fibsT_ref;
	put_binding(bindings, 18,  fibs_ref);
	{
		ref fibs;
		get_binding(bindings, 18,  &(fibs));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(fibs)
			: "rdi"
		);
		goto *(void*)print_i_list;
	}
}
ref main_function()
{
	init_constructors_Int();
	init_constructors_List();
	init_constructors_Unit();
	init_constructors_Pair();
	init_function_plus_int();
	init_function_head();
	init_function_tail();
	init_function_map();
	init_function_zip();
	init_function_uncurry();
	init_function_print_i_list();
	init_function_seq();
	ref var_98;
	var_98 = main_((ref){});
	while(stack_pointer < stack_top) {
		var_98 = case_continuation(updateContinuation(var_98));
	}
	return var_98;
}
