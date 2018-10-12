#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "stg/plus_int/static.h"
#include "stg/bindings.h"
#include "containers/mmanager.h"
#include "stg/heap.h"
#include "data/string_.h"
ref var_9_cont(hash_map* bindings);
ref var_5_cont(hash_map* bindings);
ref var_4(ref thunk_ref);
ref var_0_cont(hash_map* bindings);
ref main__slow(ref null);
ref main_();
void init_function_main_();
ref plus_int_slow(ref null);
ref plus_int(ref x1,  ref y1);
void init_function_plus_int();
void init_constructors_Unit();
void init_constructors_Int();
info_table plus_int_info_table;
ref plus_int_ref;
struct info_table I_info_table;
typedef struct I {
	info_table* info_ptr;
	int I_value;
} I;
struct info_table Unit_info_table;
typedef struct Unit {
	info_table* info_ptr;
} Unit;
void init_constructors_Int()
{
	I_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 1, .con_num = 0,  .con_name = (string_){.char_arr = "I",  .length = 1}}}};
}
void init_constructors_Unit()
{
	Unit_info_table = (info_table){.type = 1,  .extra = (union info_table_u){.constructor = (con){.arity = 0, .con_num = 0,  .con_name = (string_){.char_arr = "Unit",  .length = 4}}}};
}
ref var_0_cont(hash_map* bindings)
{
	GET_BINDING(x1_ref, void**, x1, 0, bindings)
	info_table* x1_info = *((info_table**)x1);
	int var_6 = 0;
	if(x1_info->type == 1)
	{
		if((((x1_info)->extra).constructor).con_num == 0)
		{
			I* var_7 = (I*)x1;
			NEW_REF(a1_ref, int*, sizeof(int), a1)
			*(a1) = (var_7)->I_value;
			put_binding(bindings, 8,  a1_ref);
			__asm__ volatile (
				"movq %0, %%rdi;\n\t"
				"movq %%rbp, %%rsp;\n\t"
				"popq %%rbp;\n\t"
				:
				:"r"(bindings)
			);
			goto *(void*)var_9_cont;
		}
	}
	else
	{
		assert((x1_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			:
			:"r"(x1_ref), "r"(var_0_cont), "r"(bindings), "r"(var_6),  "r"(x1_ref)
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_4(ref thunk_ref)
{
	hash_map* bindings = THUNK_GET_BINDINGS(thunk_ref);
	{
		ref pl_x;
		get_binding(bindings, 1,  &(pl_x));
		ref pl_y;
		get_binding(bindings, 2,  &(pl_y));
		return plus_int(pl_x,  pl_y);
	}
}
ref var_5_cont(hash_map* bindings)
{
	GET_BINDING(pl_z_ref, void**, pl_z, 3, bindings)
	info_table* pl_z_info = *((info_table**)pl_z);
	int var_10 = 3;
	if(pl_z_info->type == 1)
	{
		if((((pl_z_info)->extra).constructor).con_num == 0)
		{
			I* var_11 = (I*)pl_z;
			NEW_REF(pl_a_ref, int*, sizeof(int), pl_a)
			*(pl_a) = (var_11)->I_value;
			put_binding(bindings, 12,  pl_a_ref);
			GET_BINDING(var_14_ref, int*, var_14, 12, bindings)
			printf("%d\n",  *(var_14));
			NEW_REF(var_13, Unit*, sizeof(Unit), var_13_val)
			(var_13_val)->info_ptr = &Unit_info_table;
			return var_13;
		}
	}
	else
	{
		assert((pl_z_info)->type == 5);
		__asm__ volatile (
			"movq %0, %%rdi;\n\t"
			"movq %1, %%rsi;\n\t"
			"movq %2, %%rdx;\n\t"
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			:
			:"r"(pl_z_ref), "r"(var_5_cont), "r"(bindings), "r"(var_10),  "r"(pl_z_ref)
		);
		goto *(void*)thunk_continuation;
	}
}
ref var_9_cont(hash_map* bindings)
{
	GET_BINDING(y1_ref, void**, y1, 1, bindings)
	info_table* y1_info = *((info_table**)y1);
	int var_15 = 1;
	if(y1_info->type == 1)
	{
		if((((y1_info)->extra).constructor).con_num == 0)
		{
			I* var_16 = (I*)y1;
			NEW_REF(b1_ref, int*, sizeof(int), b1)
			*(b1) = (var_16)->I_value;
			put_binding(bindings, 17,  b1_ref);
			NEW_REF(c1_ref, I*, sizeof(I), c1)
			(c1)->info_ptr = &I_info_table;
			int var_19;
			{
				GET_BINDING(a1_ref, int*, a1, 8, bindings)
				GET_BINDING(b1_ref, int*, b1, 17, bindings)
				var_19 = *(a1) + *(b1);
			}
			(c1)->I_value = var_19;
			put_binding(bindings, 18,  c1_ref);
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
			"movl %3, %%ecx;\n\t"
			"movq %4, %%r8;\n\t"
			"movq %%rbp, %%rsp;\n\t"
			"popq %%rbp;\n\t"
			:
			:"r"(y1_ref), "r"(var_9_cont), "r"(bindings), "r"(var_15),  "r"(y1_ref)
		);
		goto *(void*)thunk_continuation;
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
		:"r"(bindings)
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
			:"r"(x1),  "r"(y1)
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
ref main_()
{
	hash_map * bindings;
	init_bindings(&(bindings));;
	NEW_REF(pl_x_ref, I*, sizeof(I), pl_x)
	(pl_x)->info_ptr = &I_info_table;
	(pl_x)->I_value = 2101;
	put_binding(bindings, 1,  pl_x_ref);
	NEW_REF(pl_y_ref, I*, sizeof(I), pl_y)
	(pl_y)->info_ptr = &I_info_table;
	(pl_y)->I_value = 2132;
	put_binding(bindings, 2,  pl_y_ref);
	ref pl_z_ref = create_thunk(bindings,  var_4);
	put_binding(bindings, 3,  pl_z_ref);
	__asm__ volatile (
		"movq %0, %%rdi;\n\t"
		"movq %%rbp, %%rsp;\n\t"
		"popq %%rbp;\n\t"
		:
		:"r"(bindings)
	);
	goto *(void*)var_5_cont;
}
ref main_function()
{
	init_constructors_Int();
	init_constructors_Unit();
	init_function_plus_int();
	main_((ref){});
}
