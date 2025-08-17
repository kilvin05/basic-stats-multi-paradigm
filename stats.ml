(* Functional statistics: mean, median, mode *)

let mean (xs : int list) : float =
  match xs with
  | [] -> 0.0
  | _ ->
      let sum = List.fold_left ( + ) 0 xs in
      float_of_int sum /. float_of_int (List.length xs)

let median (xs : int list) : float =
  match xs with
  | [] -> 0.0
  | _ ->
      let sorted = List.sort compare xs in
      let n = List.length sorted in
      if n mod 2 = 1 then
        float_of_int (List.nth sorted (n / 2))
      else
        let a = List.nth sorted (n / 2 - 1)
        and b = List.nth sorted (n / 2) in
        (float_of_int (a + b)) /. 2.0

module IntMap = Map.Make (Int)

let freq_map (xs : int list) : int IntMap.t =
  List.fold_left
    (fun acc x ->
      let prev = match IntMap.find_opt x acc with Some v -> v | None -> 0 in
      IntMap.add x (prev + 1) acc)
    IntMap.empty xs

let modes (xs : int list) : (int list * int) =
  match xs with
  | [] -> ([], 0)
  | _ ->
      let fm = freq_map xs in
      let (max_key, max_val) =
        IntMap.fold (fun k v acc -> match acc with
          | (k0, v0) when v > v0 -> (k, v)
          | acc -> acc) fm (0, 0)
      in
      let ms =
        IntMap.fold (fun k v acc -> if v = max_val then k :: acc else acc) fm []
      in
      (List.sort compare ms, max_val)

let () =
  (* Example input; swap for your own data or read from argv via Unix.getargv *)
  let data = [1; 2; 2; 3; 4] in
  let m = mean data in
  let med = median data in
  let (ms, f) = modes data in
  let print_list l =
    print_string "[";
    let rec loop = function
    | [] -> ()
    | [x] -> print_int x
    | x :: xs -> print_int x; print_string "; "; loop xs
    in
    loop l; print_string "]"
  in
  Printf.printf "Input: "; print_list data; print_newline ();
  Printf.printf "Mean: %.4f\n" m;
  Printf.printf "Median: %.4f\n" med;
  Printf.printf "Mode(s) (freq=%d): " f; print_list ms; print_newline ();
  ignore max_key (* prevents unused warning for max_key in some compilers *)
