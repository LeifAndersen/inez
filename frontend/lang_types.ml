type ibtype = E_Int | E_Bool
with sexp

type _ t =
| Y_Int         :  int t
| Y_Bool        :  bool t
| Y_Int_Arrow   :  'a t -> (int -> 'a) t
| Y_Bool_Arrow  :  'a t -> (bool -> 'a) t

module U = struct

  (* non-GADT version of t *)

  type t =
  | Y_Int
  | Y_Bool
  | Y_Int_Arrow   of t
  | Y_Bool_Arrow  of t
  with sexp

end

let rec ungadt_t :
type s . s t -> U.t =
  function
  | Y_Int ->
    U.Y_Int
  | Y_Bool ->
    U.Y_Bool
  | Y_Int_Arrow y ->
    U.Y_Int_Arrow (ungadt_t y)
  | Y_Bool_Arrow y ->
    U.Y_Bool_Arrow (ungadt_t y)

let rec rightmost_ibtype_of_t :
type s . s t -> ibtype =
  function
  | Y_Int -> E_Int
  | Y_Bool -> E_Bool
  | Y_Int_Arrow y -> rightmost_ibtype_of_t y
  | Y_Bool_Arrow y -> rightmost_ibtype_of_t y

let count_arrows t =
  let rec ca_aux : type s . int -> s t -> int = fun acc -> function
    | Y_Int ->
      acc
    | Y_Bool ->
      acc
    | Y_Int_Arrow y ->
      ca_aux (1 + acc) y
    | Y_Bool_Arrow y ->
      ca_aux (1 + acc) y in
  ca_aux 0 t

let t_of_app :
type r s . (r -> s) t -> r t -> s t =
  fun x y -> match x, y with
  | Y_Int_Arrow x, Y_Int ->
    x
  | Y_Bool_Arrow x, Y_Bool ->
    x

type t_box = Box : _ t -> t_box

let t_box_of_ibtype = function
  | E_Int ->
    Box Y_Int
  | E_Bool ->
    Box Y_Bool
