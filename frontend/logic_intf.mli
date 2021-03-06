module type Term = sig

  type _ atom

  type ('i, _) t =
  | M_Bool  :  'i atom Formula.t ->
    ('i, bool) t
  | M_Int   :  Core.Std.Int63.t ->
    ('i, int) t
  | M_Sum   :  ('i, int) t * ('i, int) t ->
    ('i, int) t
  | M_Prod  :  Core.Std.Int63.t * ('i, int) t ->
    ('i, int) t
  | M_Ite   :  'i atom Formula.t * ('i, int) t * ('i, int) t ->
    ('i, int) t
  | M_Var   :  ('i, 's) Id.t ->
    ('i, 's) t
  | M_App   :  ('i, 'r -> 's) t * ('i, 'r) t ->
    ('i, 's) t

end

module type Term_with_ops = sig

  include Term

  val zero : ('a, int) t
  val one : ('a, int) t

  (* conversions *)

  val of_int63 : Core.Std.Int63.t -> ('a, int) t

  val fun_id_of_app : ('i, 'r) t ->  'i Id.Box_arrow.t option

  (* type utilities *)

  val type_of_t : ('i, 's) t -> 's Type.t

  val is_int : ('i, 's) t -> bool

  val is_bool : ('i, 's) t -> bool


  (* traversal *)

  (*
  val fold_formulas :
    ('i, 's) t ->
    init : 'a ->
    f    : ('a -> 'i atom Formula.t -> 'a) ->
    'a
   *)

  val fold_sum_terms :
    ('i, int) t ->
    factor   : Core.Std.Int63.t ->
    init     : 'a ->
    f        : ('a -> Core.Std.Int63.t -> ('i, int) t -> 'a) ->
    f_offset : ('a -> Core.Std.Int63.t -> 'b) ->
    'b

  (* infix operators *)

  include (Ops_intf.Int with type ('i, 's) t := ('i, 's) t
                        and type int_plug := Core.Std.Int63.t)

end

module type Atom = sig

  type (_, _) term_plug

  type 'i t =
  | A_Bool  of  ('i, bool) term_plug
  | A_Le    of  ('i, int) term_plug
  | A_Eq    of  ('i, int) term_plug

end
