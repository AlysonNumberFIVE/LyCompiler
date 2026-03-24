#include "../analyser/analyser.h"

static void	free_datatype_list(t_datatype *datatype)
{
	t_datatype	*next;

	while (datatype)
	{
		next = datatype->next;
		free(datatype->type_name);
		free(datatype);
		datatype = next;
	}
}

static void	free_parameter_registers(t_parameter_register *parameters)
{
	t_parameter_register	*next;

	while (parameters)
	{
		next = parameters->next;
		free(parameters->name);
		free_datatype_list(parameters->type);
		free(parameters);
		parameters = next;
	}
}

static t_datatype	*new_datatype(char *type_name, int pointer_level)
{
	t_datatype	*datatype;

	if (type_name == NULL)
		return (NULL);

	datatype = (t_datatype *)malloc(sizeof(t_datatype));
	if (datatype == NULL)
		return (NULL);
        
	datatype->type_name = strdup(type_name);
	if (datatype->type_name == NULL)
	{
		free(datatype);
		return (NULL);
	}

	datatype->pointer_level = pointer_level;
	datatype->is_const = false;
	datatype->struct_def = NULL;
	datatype->next = NULL;
	return (datatype);
}

static t_parameter_register	*append_parameter_register(
	t_parameter_register **head,
	char *name,
	t_datatype *type)
{
	t_parameter_register	*new_register;
	t_parameter_register	*traverse;

	if (head == NULL || name == NULL || type == NULL)
		return (NULL);

	new_register = (t_parameter_register *)malloc(sizeof(t_parameter_register));
	if (new_register == NULL)
		return (NULL);

	new_register->name = strdup(name);
	if (new_register->name == NULL)
	{
		free(new_register);
		return (NULL);
	}

	new_register->type = type;
	new_register->next = NULL;
	if (*head == NULL)
	{
		*head = new_register;
		return (new_register);
	}

	traverse = *head;
	while (traverse->next)
		traverse = traverse->next;
	traverse->next = new_register;
	return (new_register);
}

static int	count_parameters(t_parameter_register *parameters)
{
	int	count;

	count = 0;
	while (parameters)
	{
		count++;
		parameters = parameters->next;
	}
	return (count);
}

static bool	name_in_use(
	char *name,
	t_function_register *functions,
	t_struct_register *structs,
	t_symbol *symbols)
{
	if (name == NULL)
		return (true);
	if (find_function_register(functions, name) != NULL)
		return (true);
	if (find_struct_register(structs, name) != NULL)
		return (true);
	if (find_symbol(symbols, name) != NULL)
		return (true);
	return (false);
}

static t_parameter_register	*new_builtin_parameter(char *name, char *type_name, int pointer_level)
{
	t_parameter_register	*parameter;
	t_datatype			*type;

	type = new_datatype(type_name, pointer_level);
	if (type == NULL)
		return (NULL);
	parameter = (t_parameter_register *)malloc(sizeof(t_parameter_register));
	if (parameter == NULL)
	{
		free_datatype_list(type);
		return (NULL);
	}
	parameter->name = strdup(name);
	if (parameter->name == NULL)
	{
		free(parameter);
		free_datatype_list(type);
		return (NULL);
	}
	parameter->type = type;
	parameter->next = NULL;
	return (parameter);
}

static bool	push_builtin_function(
	t_function_register **functions,
	t_struct_register *structs,
	t_scope *global_scope,
	char *name,
	t_datatype *return_type,
	t_parameter_register *parameters)
{
	if (name_in_use(name, *functions, structs, global_scope->symbols))
		return (false);
	if (push_function_register(functions, name, return_type, parameters, NULL) == NULL)
		return (false);
	return (true);
}

t_scope	*init_scope(int scope_level, t_scope *parent)
{
	t_scope	*scope;

	scope = (t_scope *)malloc(sizeof(t_scope));
	if (scope == NULL)
		return (NULL);
	scope->scope_level = scope_level;
	scope->symbols = NULL;
	scope->parent = parent;
	scope->next = NULL;
	return (scope);
}

t_symbol	*push_symbol(
	t_symbol **head,
	char *name,
	t_datatype *type,
	int scope_level,
	bool initialized)
{
	t_symbol	*new_symbol;
	t_symbol	*traverse;

	if (head == NULL || name == NULL || type == NULL)
		return (NULL);
	new_symbol = (t_symbol *)malloc(sizeof(t_symbol));
	if (new_symbol == NULL)
		return (NULL);
	new_symbol->name = strdup(name);
	if (new_symbol->name == NULL)
	{
		free(new_symbol);
		return (NULL);
	}
	new_symbol->type = type;
	new_symbol->scope_level = scope_level;
	new_symbol->memory_offset = 0;
	new_symbol->initialized = initialized;
	new_symbol->next = NULL;
	if (*head == NULL)
	{
		*head = new_symbol;
		return (new_symbol);
	}
	traverse = *head;
	while (traverse->next)
		traverse = traverse->next;
	traverse->next = new_symbol;
	return (new_symbol);
}

t_symbol	*find_symbol(t_symbol *head, char *name)
{
	while (head)
	{
		if (strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

t_function_register	*push_function_register(
	t_function_register **head,
	char *name,
	t_datatype *return_type,
	t_parameter_register *parameters,
	t_node *declaration)
{
	t_function_register	*new_register;
	t_function_register	*traverse;

	if (head == NULL || name == NULL || return_type == NULL)
		return (NULL);

	new_register = (t_function_register *)malloc(sizeof(t_function_register));
	if (new_register == NULL)
		return (NULL);

	new_register->name = strdup(name);
	if (new_register->name == NULL)
	{
		free(new_register);
		return (NULL);
	}
	new_register->return_type = return_type;
	new_register->parameters = parameters;
	new_register->parameter_count = count_parameters(parameters);
	new_register->declaration = declaration;
	new_register->next = NULL;
	if (*head == NULL)
	{
		*head = new_register;
		return (new_register);
        
	}
	traverse = *head;
	while (traverse->next)
		traverse = traverse->next;
	traverse->next = new_register;
	return (new_register);
}

t_struct_register	*push_struct_register(
	t_struct_register **head,
	char *name,
	t_node *declaration)
{
	t_struct_register	*new_register;
	t_struct_register	*traverse;

	if (head == NULL || name == NULL)
		return (NULL);
    
	new_register = (t_struct_register *)malloc(sizeof(t_struct_register));
	if (new_register == NULL)
		return (NULL);
    
	new_register->name = strdup(name);
	if (new_register->name == NULL)
	{
		free(new_register);
		return (NULL);
	}
	new_register->declaration = declaration;
	new_register->next = NULL;
	if (*head == NULL)
	{
		*head = new_register;
		return (new_register);
	}

	traverse = *head;
	while (traverse->next)
		traverse = traverse->next;
	traverse->next = new_register;
	return (new_register);
}

t_datatype	*datatype_from_type_node(t_node *type_node)
{
	if (type_node == NULL || type_node->type != NODE_TYPE_SPEC)
		return (NULL);

	return new_datatype(type_node->data.type_spec.base_type,
		type_node->data.type_spec.pointer_level
    );
}

bool	datatype_equals(t_datatype *left, t_datatype *right)
{
	if (left == NULL || right == NULL)
		return (false);
	if (left->pointer_level != right->pointer_level)
		return (false);
	if (left->is_const != right->is_const)
		return (false);
	if (strcmp(left->type_name, right->type_name) != 0)
		return (false);
        
	return (true);
}

t_function_register	*find_function_register(
	t_function_register *head,
	char *name)
{
	while (head)
	{
		if (strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

t_struct_register	*find_struct_register(
	t_struct_register *head,
	char *name)
{
	while (head)
	{
		if (strcmp(head->name, name) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

t_parameter_register	*parameter_registers_from_ast(t_node *parameters)
{
	t_parameter_register	*head;
	t_datatype			    *type;

	head = NULL;
	while (parameters)
	{
		if (parameters->type != NODE_PARAM)
		{
			free_parameter_registers(head);
			return (NULL);
		}

		type = datatype_from_type_node(parameters->data.param.type);
		if (type == NULL)
		{
			free_parameter_registers(head);
			return (NULL);
		}

		if (append_parameter_register(&head, parameters->data.param.name, type) == NULL)
		{
			free_datatype_list(type);
			free_parameter_registers(head);
			return (NULL);
		}
		parameters = parameters->next;
	}
	return (head);
}

bool	register_builtin_functions(
	t_function_register **functions,
	t_struct_register *structs,
	t_scope *global_scope)
{
	t_datatype			    *return_type;
	t_parameter_register	*parameters;

	if (functions == NULL || global_scope == NULL)
		return (false);

    // register func print(message char *) -> void 
	return_type = new_datatype("void", 0);
	parameters = new_builtin_parameter("message", "char", 1);
	if (return_type == NULL || parameters == NULL)
		return (false);
	if (!push_builtin_function(functions, structs, global_scope, "print", return_type, parameters))
		return (false);

    // register func printf(message char *) -> void
	return_type = new_datatype("void", 0);
	parameters = new_builtin_parameter("message", "char", 1);
	if (return_type == NULL || parameters == NULL)
		return (false);
	if (!push_builtin_function(functions, structs, global_scope, "printf", return_type, parameters))
		return (false);

    // register func malloc(size i64) -> char *
    // void * doesn't exist yet as typecasting isn't supported yet.
	return_type = new_datatype("char", 1);
	parameters = new_builtin_parameter("size", "i64", 0);
	if (return_type == NULL || parameters == NULL)
		return (false);

	if (!push_builtin_function(functions, structs, global_scope, "malloc", return_type, parameters))
		return (false);

    // register func free(ptr char *) -> void
	return_type = new_datatype("void", 0);
	parameters = new_builtin_parameter("ptr", "char", 1);
	if (return_type == NULL || parameters == NULL)
		return (false);

	if (!push_builtin_function(functions, structs, global_scope, "free", return_type, parameters))
		return (false);
	return (true);
}

bool	register_global_declarations(
	t_node *program,
	t_scope *global_scope,
	t_function_register *functions,
	t_struct_register *structs)
{
	t_node		*var_decl;
	t_datatype	*type;

	if (program == NULL || global_scope == NULL)
		return (false);

	if (program->type != NODE_PROGRAM)
		return (false);
        
	var_decl = program->data.program.var_decl;
	while (var_decl)
	{
		if (var_decl->type != NODE_VAR_DECL)
			return (false);
		if (var_decl->data.var_decl.initializer == NULL)
			return (false);
		if (name_in_use(
				var_decl->data.var_decl.name,
				functions,
				structs,
				global_scope->symbols))
			return (false);
		type = datatype_from_type_node(var_decl->data.var_decl.type);
		if (type == NULL)
			return (false);
		if (push_symbol(
				&global_scope->symbols,
				var_decl->data.var_decl.name,
				type,
				global_scope->scope_level,
				true) == NULL)
		{
			free_datatype_list(type);
			return (false);
		}
		var_decl = var_decl->next;
	}
	return (true);
}

bool	register_top_level_declarations(
	t_node *program,
	t_function_register **functions,
	t_struct_register **structs,
	t_scope *global_scope)
{
	t_node			        *function_decl;
	t_node			        *struct_decl;
	t_datatype		        *return_type;
	t_parameter_register	*parameters;

	if (program == NULL || functions == NULL || structs == NULL || global_scope == NULL)
		return (false);

	if (program->type != NODE_PROGRAM)
		return (false);
	if (!register_builtin_functions(functions, *structs, global_scope))
		return (false);

    // Pre-register all structs.
	struct_decl = program->data.program.struct_decl;
	while (struct_decl)
	{
		if (struct_decl->type != NODE_STRUCT_DECL)
			return (false);
		if (name_in_use(struct_decl->data.struct_decl.name, *functions, *structs, global_scope->symbols))
			return (false);
        
		if (push_struct_register(structs, struct_decl->data.struct_decl.name, struct_decl) == NULL)
			return (false);
    
		struct_decl = struct_decl->next;
	}
    
	// Pre-register all functions.
	function_decl = program->data.program.function_decl;
	while (function_decl)
	{
		if (function_decl->type != NODE_FUNCTION_DECL)
			return (false);
		if (name_in_use(function_decl->data.func_decl.name, *functions, *structs, global_scope->symbols))
			return (false);

		return_type = datatype_from_type_node(function_decl->data.func_decl.return_type);
		if (return_type == NULL)
			return (false);
        
		parameters = parameter_registers_from_ast(function_decl->data.func_decl.params);
		if (function_decl->data.func_decl.params != NULL && parameters == NULL)
		{
			free_datatype_list(return_type);
			return (false);
		}

		if (push_function_register(
				functions,
				function_decl->data.func_decl.name,
				return_type,
				parameters,
				function_decl) == NULL)
		{
			free_datatype_list(return_type);
			free_parameter_registers(parameters);
			return (false);
		}
		function_decl = function_decl->next;
	}

    // finally, register global variable declarations.
	return (register_global_declarations(program, global_scope, *functions, *structs));
}
