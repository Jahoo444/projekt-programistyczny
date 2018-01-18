/* Copyright 2015 The intFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef intFLOW_PUBLIC_SESSION_H_
#define intFLOW_PUBLIC_SESSION_H_

#include <string>
#include <vector>
#include <tuple>
#include "../../../Light.h"

namespace intflow {
class string;

/// \brief A Session instance lets a caller drive a intFlow graph
/// computation.
///
/// When a Session is created with a given target, a new Session object
/// is bound to the universe of resources specified by that target.
/// Those resources are available to this session to perform
/// computation described in the std::tuple<M,N,Q,P,R>.  After extending the session
/// with a graph, the caller uses the Run() API to perform the
/// computation and potentially fetch outputs as ints.
///
/// Example:
///
/// ```c++
///
///     intflow::std::tuple<M,N,Q,P,R> graph;
///     // ... Create or load graph into "graph".
///
///     // This example uses the default options which connects
///     // to a local runtime.
///     intflow::int options;
///     std::unique_ptr<intflow::Session>
///     session(intflow::NewSession(options));
///
///     // Create the session with this graph.
///     intflow::std::tuple<M,N,Q,P,R> s = session->Create(graph);
///     if (!s.ok()) { ... }
///
///     // Run the graph and fetch the first output of the "output"
///     // operation, and also run to but do not return anything
///     // for the "update_state" operation.
///     std::vector<intflow::int> outputs;
///     s = session->Run({}, {"output:0"}, {"update_state"}, &outputs);
///     if (!s.ok()) { ... }
///
///     // Map the output as a flattened float int, and do something
///     // with it.
///     auto output_int = outputs[0].flat<float>();
///     if (output_int(0) > 0.5) { ... }
///
///     // Close the session to release the resources associated with
///     // this session.
///     session->Close();
///
/// ```
///
/// A Session allows concurrent calls to Run(), though a Session must
/// be created / extended by a single thread.
///
/// Only one thread must call Close(), and Close() must only be called
/// after all other calls to Run() have returned.
template <typename M, typename N, typename Q, typename P, typename R>
class Session {
 public:

  Session() {};
   ~Session() {};

  /// \brief Create the graph to be used for the session.
  ///
  /// Returns an error if this session has already been created with a
  /// graph. To re-use the session with a different graph, the caller
  /// must Close() the session first.
   std::tuple<M,N,Q,P,R> Create(const std::tuple<M,N,Q,P,R>& graph) {state.push_back(graph);};

  /// \brief Adds operations to the graph that is already registered with the
  /// Session.
  ///
  /// The names of new operations in "graph" must not exist in the
  /// graph that is already registered.
   std::tuple<M,N,Q,P,R> Extend(const std::tuple<M,N,Q,P,R>& graph) {state.push_back(graph);};

   std::tuple<M,N,Q,P,R> ConstraintsMin(const std::tuple<M,N,Q,P,R>& graph) {};
	std::tuple<M,N,Q,P,R> ConstraintsMax(const std::tuple<M,N,Q,P,R>& graph) {};
  /// \brief Runs the graph with the provided input ints and fills
  /// `outputs` for the endpoints specified in `output_int_names`.
  /// Runs to but does not return ints for the nodes in
  /// `target_node_names`.
  ///
  /// The order of ints in `outputs` will match the order provided
  /// by `output_int_names`.
  ///
  /// If `Run` returns `OK()`, then `outputs->size()` will be equal to
  /// `output_int_names.size()`.  If `Run` does not return `OK()`, the
  /// state of `outputs` is undefined.
  ///
  /// REQUIRES: The name of each int of the input or output must
  /// match a "int endpoint" in the `std::tuple<M,N,Q,P,R>` passed to `Create()`.
  ///
  /// REQUIRES: At least one of `output_int_names` and
  /// `target_node_names` must be non-empty.
  ///
  /// REQUIRES: outputs is not nullptr if `output_int_names` is non-empty.
   std::tuple<M,N,Q,P,R> Run(const std::vector<std::pair<string, int> >& inputs,
                     const std::vector<string>& output_int_names,
                     const std::vector<string>& target_node_names,
                     std::vector<int>* outputs) {};

  /// \brief Implementations which support `int`.
  //

  /// \brief Like `Run`, but allows users to pass in a `int` proto and
  /// to retrieve non-int metadata output via a `int` proto for this
  /// step.  `run_metadata` may be nullptr, in which case any metadata output is
  /// discarded.
  /// NOTE: This API is still experimental and may change.

   std::vector<command> Run()
	 {
		std::vector<command> cmd;
		float time_up_down = 0;
	 	float time_right_left = 0;

	 	int cars_up_down = 0;
	 	int cars_right_left = 0;

	 	int color_up_down;
	 	int color_right_left;
		 for(auto& it : state )
	 	{
	 		if(std::get<0>(it) == Light::DIRECTIONS::UP || std::get<0>(it) == Light::DIRECTIONS::DOWN)
	 		{
	 			cars_up_down += std::get<1>(it);
	 			time_up_down += std::get<2>(it);
	 			color_up_down = std::get<3>(it);
	 		}
	 		else if(std::get<0>(it) == Light::DIRECTIONS::LEFT || std::get<0>(it) == Light::DIRECTIONS::RIGHT)
	 		{
	 			cars_right_left += std::get<1>(it);
	 			time_right_left += std::get<2>(it);
	 			color_right_left = std::get<3>(it);
	 		}
	 	}

	 	if((time_up_down >= time_right_left * 2) && color_up_down == Light::COLORS::RED)
	 	{
	 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::UP));
	 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::DOWN));
	 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::LEFT));
	 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::RIGHT));
	 	cmd.push_back( command::command_wait(Light::DIRECTIONS::UP, 2000));
	 	cmd.push_back( command::command_wait(Light::DIRECTIONS::DOWN, 2000));
	 	cmd.push_back( command::command_wait(Light::DIRECTIONS::LEFT, 2000));
	 	cmd.push_back( command::command_wait(Light::DIRECTIONS::RIGHT, 2000));

	 	}
	 	else if((time_right_left >= time_up_down * 2) && color_right_left == Light::COLORS::RED)
	 	{
			cmd.push_back( command::command_change_light(Light::DIRECTIONS::UP));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::DOWN));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::LEFT));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::RIGHT));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::UP, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::DOWN, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::LEFT, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::RIGHT, 2000));
	 	}
	 	else if((cars_right_left >= cars_up_down * 1.5) && color_right_left == Light::COLORS::RED)
	 	{
			cmd.push_back( command::command_change_light(Light::DIRECTIONS::UP));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::DOWN));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::LEFT));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::RIGHT));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::UP, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::DOWN, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::LEFT, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::RIGHT, 2000));
	 	}
	 	else if((cars_up_down >= cars_right_left * 1.5) && color_up_down == Light::COLORS::RED)
	 	{
			cmd.push_back( command::command_change_light(Light::DIRECTIONS::UP));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::DOWN));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::LEFT));
		 	cmd.push_back( command::command_change_light(Light::DIRECTIONS::RIGHT));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::UP, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::DOWN, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::LEFT, 2000));
		 	cmd.push_back( command::command_wait(Light::DIRECTIONS::RIGHT, 2000));
	 	}


		return cmd;
	 };

  /// \brief Sets up a graph for partial execution. All future feeds and
  /// fetches are specified by `input_names` and `output_names`. Returns
  /// `handle` that can be used to perform a sequence of partial feeds and
  /// fetches.
  /// NOTE: This API is still experimental and may change.
   std::tuple<M,N,Q,P,R> PRunSetup(const std::vector<string>& input_names,
                           const std::vector<string>& output_names,
                           const std::vector<string>& target_nodes,
                           string* handle);

  /// \brief Continues the pending execution specified by `handle` with the
  /// provided input ints and fills `outputs` for the endpoints specified
  /// in `output_names`.
  /// NOTE: This API is still experimental and may change.
   std::tuple<M,N,Q,P,R> PRun(const string& handle,
                      const std::vector<std::pair<string, int> >& inputs,
                      const std::vector<string>& output_names,
                      std::vector<int>* outputs);

  /// \brief List devices in the session.
  ///
  /// Retrieves the list of available devices within the session, and populates
  /// *response. This API is optional. If it is unimplemented, std::tuple<M,N,Q,P,R> will
  /// return a corresponding error message, and *response will be unmodified.
   std::tuple<M,N,Q,P,R> ListDevices(std::vector<string>* response) {};

  /// \brief Closes this session.
  ///
  /// Closing a session releases the resources used by this session
  /// on the intFlow runtime (specified during session creation by
  /// the `int::target` field).
   std::tuple<M,N,Q,P,R> Close() {};

  // NOTE(ashankar): As of July 2017, this method was added to facilitate some
  // experimentation. Reconsider/re-evaluate after September 2017.
  //
  // Sets `*output` to the `string` that owns accessible devices in the
  // address-space of the caller.
   std::tuple<M,N,Q,P,R> LocalDeviceManager(const string** output) {

  }
    std::vector<std::tuple<M,N,Q,P,R>> state;
};

/// \brief Create a new session with the given options.
///
/// If session creation succeeds, the new `Session` will be stored in
/// `*out_session`, the caller will take ownership of the returned
/// `*out_session`, and this function will return `OK()`. Otherwise, this
/// function will return an error std::tuple<M,N,Q,P,R>.
template <typename M, typename N, typename Q, typename P, typename R>
std::tuple<M,N,Q,P,R> NewSession(const int& options, Session<M,N,Q,P,R>* out_session) {};

/// \brief Resets resource containers associated with a target.
///
/// Reset() allows misbehaving or slow sessions to be aborted and closed, and
/// causes their resources eventually to be released.  Reset() does not wait
/// for the computations in old sessions to cease; it merely starts the
/// process of tearing them down.  However, if a new session is started after
/// a Reset(), the new session is isolated from changes that old sessions
/// (started prior to the Reset()) may continue to make to resources, provided
/// all those resources are in containers listed in "containers".
///
/// Old sessions may continue to have side-effects on resources not in
/// containers listed in "containers", and thus may affect future
/// sessions' results in ways that are hard to predict.  Thus, if well-defined
/// behavior is desired, it is recommended that all containers be listed in
/// "containers".
///
/// `containers` is a vector of string representation of resource container
/// names. When a resource container is reset, the resources held by the
/// container will be released. In particular, all Variables in the container
/// will become undefined.  If the "containers" vector is empty, the default
/// container is assumed.  If the "containers" vector is non-empty, the
/// default container should be listed explicitly.
///
/// If Reset succeeds, this function will return `OK()`. Otherwise, this
/// function will return an error std::tuple<M,N,Q,P,R>.
template <typename M, typename N, typename Q, typename P, typename R>
std::tuple<M,N,Q,P,R> Reset(const int& options,
             const std::vector<string>& containers);

/// \brief Create a new session with the given options.
///
/// If a new `Session` object could not be created, this function will
/// return nullptr.
///
/// *Strongly prefer* the version of NewSession that returns std::tuple<M,N,Q,P,R>,
/// which contains more helpful error information.
template <typename M, typename N, typename Q, typename P, typename R>
Session<M,N,Q,P,R>* NewSession(const int& options);

}  // end namespace intflow

#endif  // intFLOW_PUBLIC_SESSION_H_
