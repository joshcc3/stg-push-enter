#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stg/plus_int/static.h"
#include "stg/bindings.h"
#include "containers/mmanager.h"
#include "stg/heap.h"
#include "data/string_.h"
ref var_90(ref thunk_ref);
ref var_58_cont(hash_map* bindings);
ref var_48(ref thunk_ref);
ref var_46(ref thunk_ref);
ref var_28_cont(hash_map* bindings);
ref var_24_cont(hash_map* bindings);
ref var_23(ref thunk_ref);
ref var_19(ref thunk_ref);
ref var_16(ref thunk_ref);
ref var_14(ref thunk_ref);
ref var_9(ref thunk_ref);
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
ref index_slow(ref null);
ref index(ref in_l,  int in_x);
void init_function_index();
ref seq_slow(ref null);
ref seq(ref se_a,  ref se_b);
void init_function_seq();
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
void init_constructors_Bool();
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
info_table seq_info_table;
ref seq_ref;
info_table index_info_table;
ref index_ref;
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
struct info_table True_info_table;
typedef struct True {
	info_table* info_ptr;
} True;
struct info_table False_info_table;
typedef struct False {
	info_table* info_ptr;
} False;
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
void init_constructors_Bool()
{
	True_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 0, .con_num = 0,  .con_name = (string_){.char_arr = "True",  .length = 4}}}};
	False_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 0, .con_num = 1,  .con_name = (string_){.char_arr = "False",  .length = 5}}}};
}
ref var_0_cont(hash_map* bindings)
{
	GET_BINDING(x1_ref, void**, x1, 0, bindings)
	info_table* x1_info = *((info_table**)x1);
	int var_25 = 0;
	if(x1_info->type == 1)
	{
		if((((x1_info)->extra).constructor).con_num == 0)
		{
			I* var_26 = (I*)x1;
			NEW_REF(a1_ref, int*, sizeof(int), a1)
			*(a1) = (var_26)->I_value;
			put_binding(bindings, 27,  a1_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_28_cont;
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
			: "r"(x1_ref), "r"(var_0_cont), "r"(bindings), "r"(var_25),  "r"(x1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_1_cont(hash_map* bindings)
{
	GET_BINDING(he_l_ref, void**, he_l, 0, bindings)
	info_table* he_l_info = *((info_table**)he_l);
	int var_29 = 0;
	if(he_l_info->type == 1)
	{
		if((((he_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_30 = (Cons*)he_l;
			ref he_v_ref = (var_30)->C_element;
			put_binding(bindings, 31,  he_v_ref);
			ref he_n_ref = (var_30)->C_next;
			put_binding(bindings, 32,  he_n_ref);
			return he_v_ref;
		}
		if((((he_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_33 = (Nil*)he_l;
			assert(false);
			ref var_34;
			return var_34;
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
			: "r"(he_l_ref), "r"(var_1_cont), "r"(bindings), "r"(var_29),  "r"(he_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_2_cont(hash_map* bindings)
{
	GET_BINDING(ta_l_ref, void**, ta_l, 0, bindings)
	info_table* ta_l_info = *((info_table**)ta_l);
	int var_35 = 0;
	if(ta_l_info->type == 1)
	{
		if((((ta_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_36 = (Cons*)ta_l;
			ref ta_v_ref = (var_36)->C_element;
			put_binding(bindings, 37,  ta_v_ref);
			ref ta_n_ref = (var_36)->C_next;
			put_binding(bindings, 38,  ta_n_ref);
			return ta_n_ref;
		}
		if((((ta_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_39 = (Nil*)ta_l;
			assert(false);
			ref var_40;
			return var_40;
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
			: "r"(ta_l_ref), "r"(var_2_cont), "r"(bindings), "r"(var_35),  "r"(ta_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_3_cont(hash_map* bindings)
{
	GET_BINDING(ma_l_ref, void**, ma_l, 1, bindings)
	info_table* ma_l_info = *((info_table**)ma_l);
	int var_41 = 1;
	if(ma_l_info->type == 1)
	{
		if((((ma_l_info)->extra).constructor).con_num == 0)
		{
			Cons* var_42 = (Cons*)ma_l;
			ref ma_v_ref = (var_42)->C_element;
			put_binding(bindings, 43,  ma_v_ref);
			ref ma_n_ref = (var_42)->C_next;
			put_binding(bindings, 44,  ma_n_ref);
			ref ma_new_v_ref = create_thunk(bindings,  var_46);
			put_binding(bindings, 45,  ma_new_v_ref);
			ref ma_new_n_ref = create_thunk(bindings,  var_48);
			put_binding(bindings, 47,  ma_new_n_ref);
			NEW_REF(ma_new_res_ref, Cons*, sizeof(Cons), ma_new_res)
			(ma_new_res)->info_ptr = &Cons_info_table;
			(ma_new_res)->C_element = ma_new_v_ref;
			(ma_new_res)->C_next = ma_new_n_ref;
			put_binding(bindings, 49,  ma_new_res_ref);
			return ma_new_res_ref;
		}
		if((((ma_l_info)->extra).constructor).con_num == 1)
		{
			Nil* var_50 = (Nil*)ma_l;
			NEW_REF(ma_z_ref, Nil*, sizeof(Nil), ma_z)
			(ma_z)->info_ptr = &Nil_info_table;
			put_binding(bindings, 51,  ma_z_ref);
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
			: "r"(ma_l_ref), "r"(var_3_cont), "r"(bindings), "r"(var_41),  "r"(ma_l_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_4_cont(hash_map* bindings)
{
	GET_BINDING(zi_as_ref, void**, zi_as, 0, bindings)
	info_table* zi_as_info = *((info_table**)zi_as);
	int var_52 = 0;
	if(zi_as_info->type == 1)
	{
		if((((zi_as_info)->extra).constructor).con_num == 1)
		{
			Nil* var_53 = (Nil*)zi_as;
			NEW_REF(zi_nil_ref, Nil*, sizeof(Nil), zi_nil)
			(zi_nil)->info_ptr = &Nil_info_table;
			put_binding(bindings, 54,  zi_nil_ref);
			return zi_nil_ref;
		}
		if((((zi_as_info)->extra).constructor).con_num == 0)
		{
			Cons* var_55 = (Cons*)zi_as;
			ref zi_a_ref = (var_55)->C_element;
			put_binding(bindings, 56,  zi_a_ref);
			ref zi_an_ref = (var_55)->C_next;
			put_binding(bindings, 57,  zi_an_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				: 
				: "r"(bindings)
				: "rdi"
			);
			goto *(void*)var_58_cont;
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
			: "r"(zi_as_ref), "r"(var_4_cont), "r"(bindings), "r"(var_52),  "r"(zi_as_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_5_cont(hash_map* bindings)
{
	GET_BINDING(un_p_ref, void**, un_p, 1, bindings)
	info_table* un_p_info = *((info_table**)un_p);
	int var_59 = 1;
	if(un_p_info->type == 1)
	{
		if((((un_p_info)->extra).constructor).con_num == 0)
		{
			Pa* var_60 = (Pa*)un_p;
			ref un_a_ref = (var_60)->P_fst;
			put_binding(bindings, 61,  un_a_ref);
			ref un_b_ref = (var_60)->P_snd;
			put_binding(bindings, 62,  un_b_ref);
			GET_BINDING(un_f_ref, void**, un_f, 0, bindings)
			int var_64 = 0;
			info_table un_f_info = *(*((info_table**)un_f));
			if(un_f_info.type == 0)
			{
				if((((un_f_info).layout).entries[1]).pointer)
				{
					ref un_b_ref;
					get_binding(bindings, 62,  &(un_b_ref));
					push_ptr(un_b_ref);
				}
				else
				{
					GET_BINDING(un_b_ref, int*, un_b, 62, bindings)
					push_int(*(un_b));
				}
				if((((un_f_info).layout).entries[0]).pointer)
				{
					ref un_a_ref;
					get_binding(bindings, 61,  &(un_a_ref));
					push_ptr(un_a_ref);
				}
				else
				{
					GET_BINDING(un_a_ref, int*, un_a, 61, bindings)
					push_int(*(un_a));
				}
				ref var_63;
				void* var_65 = (void*)(((un_f_info).extra).function).slow_entry_point;
				__asm__ volatile   goto (
					"movq %0, %%rdi;\n\t"
					"movq %1, %%rsi;\n\t"
					"movq %%rbp, %%rsp;\n\t"
					"popq %%rbp;\n\t"
					"jmp *%1;\n\t"
					: 
					: "r"(var_63),  "r"(var_65)
					: "rdi",  "rsi"
				);
			}
			else if(un_f_info.type == 4)
				{
					if((((*((((un_f_info).extra).pap_info).info_ptr)).layout).entries[1]).pointer)
					{
						ref un_b_ref;
						get_binding(bindings, 62,  &(un_b_ref));
						push_ptr(un_b_ref);
					}
					else
					{
						GET_BINDING(un_b_ref, int*, un_b, 62, bindings)
						push_int(*(un_b));
					}
					if((((*((((un_f_info).extra).pap_info).info_ptr)).layout).entries[0]).pointer)
					{
						ref un_a_ref;
						get_binding(bindings, 61,  &(un_a_ref));
						push_ptr(un_a_ref);
					}
					else
					{
						GET_BINDING(un_a_ref, int*, un_a, 61, bindings)
						push_int(*(un_a));
					}
					unroll_pap(un_f);
					ref var_63;
					void* var_65 = (void*)(((*((((un_f_info).extra).pap_info).info_ptr)).extra).function).slow_entry_point;
					__asm__ volatile   goto (
						"movq %0, %%rdi;\n\t"
						"movq %1, %%rsi;\n\t"
						"movq %%rbp, %%rsp;\n\t"
						"popq %%rbp;\n\t"
						"jmp *%1;\n\t"
						: 
						: "r"(var_63),  "r"(var_66)
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
					: "r"(un_f_ref), "r"(var_5_cont), "r"(bindings), "r"(var_64),  "r"(un_f_ref)
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
			: "r"(un_p_ref), "r"(var_5_cont), "r"(bindings), "r"(var_59),  "r"(un_p_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_6_cont(hash_map* bindings)
{
	GET_BINDING(se_a_ref, void**, se_a, 0, bindings)
	info_table* se_a_info = *((info_table**)se_a);
	int var_67 = 0;
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
			: "r"(se_a_ref), "r"(var_6_cont), "r"(bindings), "r"(var_67),  "r"(se_a_ref)
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
		ref in_l;
		get_binding(bindings, 0,  &(in_l));
		return tail(in_l);
	}
}
ref var_14(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref fibs;
		get_binding(bindings, 21,  &(fibs));
		ref fibTail;
		get_binding(bindings, 15,  &(fibTail));
		return zip(fibs,  fibTail);
	}
}
ref var_16(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref fibs;
		get_binding(bindings, 21,  &(fibs));
		return tail(fibs);
	}
}
ref var_19(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref plus_uncurried;
		get_binding(bindings, 17,  &(plus_uncurried));
		ref zipped;
		get_binding(bindings, 13,  &(zipped));
		return map(plus_uncurried,  zipped);
	}
}
ref var_23(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref fibs;
		get_binding(bindings, 21,  &(fibs));
		return index(fibs,  20);
	}
}
ref var_24_cont(hash_map* bindings)
{
	GET_BINDING(elem_ref, void**, elem, 22, bindings)
	info_table* elem_info = *((info_table**)elem);
	int var_68 = 22;
	if(elem_info->type == 1)
	{
		if((((elem_info)->extra).constructor).con_num == 0)
		{
			I* var_69 = (I*)elem;
			NEW_REF(x_ref, int*, sizeof(int), x)
			*(x) = (var_69)->I_value;
			put_binding(bindings, 70,  x_ref);
			GET_BINDING(var_72_ref, int*, var_72, 70, bindings)
			printf("%d\n",  *(var_72));
			NEW_REF(var_71, Unit*, sizeof(Unit), var_71_val)
			(var_71_val)->info_ptr = &Unit_info_table;
			return var_71;
		}
	}
	else
	{
		assert((elem_info)->type == 5);
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
			: "r"(elem_ref), "r"(var_24_cont), "r"(bindings), "r"(var_68),  "r"(elem_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_28_cont(hash_map* bindings)
{
	GET_BINDING(y1_ref, void**, y1, 1, bindings)
	info_table* y1_info = *((info_table**)y1);
	int var_73 = 1;
	if(y1_info->type == 1)
	{
		if((((y1_info)->extra).constructor).con_num == 0)
		{
			I* var_74 = (I*)y1;
			NEW_REF(b1_ref, int*, sizeof(int), b1)
			*(b1) = (var_74)->I_value;
			put_binding(bindings, 75,  b1_ref);
			NEW_REF(c1_ref, I*, sizeof(I), c1)
			(c1)->info_ptr = &I_info_table;
			int var_77;
			{
				GET_BINDING(a1_ref, int*, a1, 27, bindings)
				GET_BINDING(b1_ref, int*, b1, 75, bindings)
				var_77 = *(a1) + *(b1);
			}
			(c1)->I_value = var_77;
			put_binding(bindings, 76,  c1_ref);
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
			: "r"(y1_ref), "r"(var_28_cont), "r"(bindings), "r"(var_73),  "r"(y1_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_46(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	GET_BINDING(ma_f_ref, void**, ma_f, 0, bindings)
	int var_79 = 0;
	info_table ma_f_info = *(*((info_table**)ma_f));
	if(ma_f_info.type == 0)
	{
		if((((ma_f_info).layout).entries[0]).pointer)
		{
			ref ma_v_ref;
			get_binding(bindings, 43,  &(ma_v_ref));
			push_ptr(ma_v_ref);
		}
		else
		{
			GET_BINDING(ma_v_ref, int*, ma_v, 43, bindings)
			push_int(*(ma_v));
		}
		ref var_78;
		void* var_80 = (void*)(((ma_f_info).extra).function).slow_entry_point;
		__asm__ volatile   goto (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			"jmp *%1;\n\t"
			: 
			: "r"(var_78),  "r"(var_80)
			: "rdi",  "rsi"
		);
	}
	else if(ma_f_info.type == 4)
		{
			if((((*((((ma_f_info).extra).pap_info).info_ptr)).layout).entries[0]).pointer)
			{
				ref ma_v_ref;
				get_binding(bindings, 43,  &(ma_v_ref));
				push_ptr(ma_v_ref);
			}
			else
			{
				GET_BINDING(ma_v_ref, int*, ma_v, 43, bindings)
				push_int(*(ma_v));
			}
			unroll_pap(ma_f);
			ref var_78;
			void* var_80 = (void*)(((*((((ma_f_info).extra).pap_info).info_ptr)).extra).function).slow_entry_point;
			__asm__ volatile   goto (
				"movq %0, %%rdi;\n\t"
				"movq %1, %%rsi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				"jmp *%1;\n\t"
				: 
				: "r"(var_78),  "r"(var_81)
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
			: "r"(ma_f_ref), "r"(var_46), "r"(bindings), "r"(var_79),  "r"(ma_f_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_48(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref ma_f;
		get_binding(bindings, 0,  &(ma_f));
		ref ma_n;
		get_binding(bindings, 44,  &(ma_n));
		return map(ma_f,  ma_n);
	}
}
ref var_58_cont(hash_map* bindings)
{
	GET_BINDING(zi_bs_ref, void**, zi_bs, 1, bindings)
	info_table* zi_bs_info = *((info_table**)zi_bs);
	int var_82 = 1;
	if(zi_bs_info->type == 1)
	{
		if((((zi_bs_info)->extra).constructor).con_num == 1)
		{
			Nil* var_83 = (Nil*)zi_bs;
			NEW_REF(zi_nil_ref, Nil*, sizeof(Nil), zi_nil)
			(zi_nil)->info_ptr = &Nil_info_table;
			put_binding(bindings, 84,  zi_nil_ref);
			return zi_nil_ref;
		}
		if((((zi_bs_info)->extra).constructor).con_num == 0)
		{
			Cons* var_85 = (Cons*)zi_bs;
			ref zi_b_ref = (var_85)->C_element;
			put_binding(bindings, 86,  zi_b_ref);
			ref zi_bn_ref = (var_85)->C_next;
			put_binding(bindings, 87,  zi_bn_ref);
			NEW_REF(zi_p_ref, Pa*, sizeof(Pa), zi_p)
			(zi_p)->info_ptr = &Pa_info_table;
			GET_BINDING(zi_a_ref, void**, zi_a, 56, bindings)
			(zi_p)->P_fst = zi_a_ref;
			(zi_p)->P_snd = zi_b_ref;
			put_binding(bindings, 88,  zi_p_ref);
			ref zi_rest_ref = create_thunk(bindings,  var_90);
			put_binding(bindings, 89,  zi_rest_ref);
			NEW_REF(zi_res_ref, Cons*, sizeof(Cons), zi_res)
			(zi_res)->info_ptr = &Cons_info_table;
			(zi_res)->C_element = zi_p_ref;
			(zi_res)->C_next = zi_rest_ref;
			put_binding(bindings, 91,  zi_res_ref);
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
			: "r"(zi_bs_ref), "r"(var_58_cont), "r"(bindings), "r"(var_82),  "r"(zi_bs_ref)
			: "rdi", "rsi", "rdx", "rcx",  "r8"
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_90(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref zi_an;
		get_binding(bindings, 57,  &(zi_an));
		ref zi_bn;
		get_binding(bindings, 87,  &(zi_bn));
		return zip(zi_an,  zi_bn);
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
	goto *(void*)var_6_cont;
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
ref index(ref in_l,  int in_x)
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	put_binding(bindings, 0,  in_l);
	ref in_x_ref;
	new_ref(sizeof(int),  &(in_x_ref));
	*((int*)get_ref(in_x_ref)) = in_x;
	put_binding(bindings, 1,  in_x_ref);
	int var_7;
	{
		GET_BINDING(in_x_ref, int*, in_x, 1, bindings)
		var_7 = *(in_x) == 0;
	}
	if(var_7 == 1)
	{
		{
			ref in_l;
			get_binding(bindings, 0,  &(in_l));
			return head(in_l);
		}
	}
	if(var_7 == 0)
	{
		ref in_n_ref = create_thunk(bindings,  var_9);
		put_binding(bindings, 8,  in_n_ref);
		{
			ref in_n;
			get_binding(bindings, 8,  &(in_n));
			int var_10;
			{
				GET_BINDING(in_x_ref, int*, in_x, 1, bindings)
				var_10 = *(in_x) - 1;
			}
			return index(in_n,  var_10);
		}
	}
}
ref index_slow(ref null)
{
	if(arg_satisfaction_check(sizeof(ref)+  sizeof(int)))
	{
		ref in_l;
		int in_x;
		pop_ptr(&(in_l));
		pop_int(&(in_x));
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"xorq %%rsi, %%rsi\n\t"
			"movl %1, %%esi;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			: 
			: "r"(in_l),  "r"(in_x)
			: "rdi",  "rsi"
		);
		goto *(void*)index;
	}
	else if(arg_satisfaction_check(sizeof(ref)))
		{
			ref in_l;
			pop_ptr(&(in_l));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(index_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = in_l;
			pap_[0] = pap__info;
		}
	else if(arg_satisfaction_check(sizeof(ref)+  sizeof(int)))
		{
			ref in_l;
			int in_x;
			pop_ptr(&(in_l));
			pop_int(&(in_x));
			struct info_table* pap__info = (struct info_table*)new(sizeof(info_table));
			(pap__info)->type = 4;
			(pap__info)->extra.pap_info = (struct pap){.info_ptr = &(index_info_table),  .size = 1};
			NEW_REF(pap__ref, void**, sizeof(ref)+  sizeof(int) + sizeof(void*), pap_)
			*((ref*)((char*)pap_ + 0 + sizeof(ref))) = in_l;
			*((int*)((char*)pap_ + 0 + sizeof(ref) + sizeof(int))) = in_x;
			pap_[0] = pap__info;
		}
	else
	{
		assert(false);
	}
}
void init_function_index()
{
	arg_entry* layout_entries = (arg_entry*)new(sizeof(arg_entry)*2);
	layout_entries[0] = (arg_entry){.size = sizeof(ref), .pointer = true,  .offset = 0};
	layout_entries[1] = (arg_entry){.size = sizeof(int), .pointer = false,  .offset = 0 + sizeof(ref)};
	index_info_table = (info_table){.type = 0, .extra = (fun){.slow_entry_point = index_slow,  .arity = 2},  .layout = (struct layout){.num = 2,  .entries = layout_entries}};
	new_ref(sizeof(void*),  &(index_ref));
	*((info_table**)get_ref(index_ref)) = &(index_info_table);
}
ref main_()
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	NEW_REF(one_ref, I*, sizeof(I), one)
	(one)->info_ptr = &I_info_table;
	(one)->I_value = 1;
	put_binding(bindings, 11,  one_ref);
	NEW_REF(zero_ref, I*, sizeof(I), zero)
	(zero)->info_ptr = &I_info_table;
	(zero)->I_value = 0;
	put_binding(bindings, 12,  zero_ref);
	ref zipped_ref = create_thunk(bindings,  var_14);
	put_binding(bindings, 13,  zipped_ref);
	ref fibTail_ref = create_thunk(bindings,  var_16);
	put_binding(bindings, 15,  fibTail_ref);
	struct info_table* plus_uncurried_info = (struct info_table*)new(sizeof(info_table));
	(plus_uncurried_info)->type = 4;
	(plus_uncurried_info)->extra.pap_info = (struct pap){.info_ptr = &(uncurry_info_table),  .size = 1};
	NEW_REF(plus_uncurried_ref, void**, sizeof(ref) + sizeof(void*), plus_uncurried)
	*((ref*)((char*)plus_uncurried + 0 + sizeof(ref))) = plus_int_ref;
	plus_uncurried[0] = plus_uncurried_info;
	put_binding(bindings, 17,  plus_uncurried_ref);
	ref mapped_ref = create_thunk(bindings,  var_19);
	put_binding(bindings, 18,  mapped_ref);
	NEW_REF(fibsT_ref, Cons*, sizeof(Cons), fibsT)
	(fibsT)->info_ptr = &Cons_info_table;
	(fibsT)->C_element = one_ref;
	(fibsT)->C_next = mapped_ref;
	put_binding(bindings, 20,  fibsT_ref);
	NEW_REF(fibs_ref, Cons*, sizeof(Cons), fibs)
	(fibs)->info_ptr = &Cons_info_table;
	(fibs)->C_element = zero_ref;
	(fibs)->C_next = fibsT_ref;
	put_binding(bindings, 21,  fibs_ref);
	ref elem_ref = create_thunk(bindings,  var_23);
	put_binding(bindings, 22,  elem_ref);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		: 
		: "r"(bindings)
		: "rdi"
	);
	goto *(void*)var_24_cont;
}
ref main_function()
{
	init_constructors_Int();
	init_constructors_List();
	init_constructors_Unit();
	init_constructors_Pair();
	init_constructors_Bool();
	init_function_plus_int();
	init_function_head();
	init_function_tail();
	init_function_map();
	init_function_zip();
	init_function_uncurry();
	init_function_seq();
	init_function_index();
	main_((ref){});
}
