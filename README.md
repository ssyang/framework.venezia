# framework.venezia
my C++ AI framework

* class hierarchy

* c_var_base
  * MatrixXd m_matrix_data
  * MatrixXd m_matrix_grad
* _c_fun_base
  * include c_var_base
  * c_var_base m_var_in
  * m_var_in = x at _forward()
* c_fun_base_t : public _c_fun_base
  * template class

* c_variable : public c_var_base
  * include c_fun_base
  * set_creator

* c_function : public _venezia::c_fun_base_t<c_function>
  * include c_variable

